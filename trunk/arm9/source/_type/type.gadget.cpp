#include "_type/type.gadget.h"
#include "_type/type.gadget.screen.h"
#include "_type/type.system.h"
#include "_type/type.timer.h"
#include "_gadget/gadget.button.h"

_array<_staticCallback<_eventHandler>,13> _gadget::defaultEventHandlers = {
	{
		/* onDraw */ make_callback( &_gadget::gadgetRefreshHandler ),
		/* onMouseClick */ make_callback( &_gadget::gadgetMouseHandler ),
		/* onMouseDoubleClick */ make_callback( &_gadget::gadgetMouseHandler ),
		/* onMouseDown */ make_callback( &_gadget::gadgetMouseHandler ),
		/* onMouseUp */ make_callback( &_gadget::gadgetMouseHandler ),
		/* onMouseRepeat */ make_callback( &_gadget::gadgetMouseHandler ),
		/* onKeyDown */ make_callback( &_gadget::gadgetKeyHandler ),
		/* onKeyUp */ make_callback( &_gadget::gadgetKeyHandler ),
		/* onKeyClick */ make_callback( &_gadget::gadgetKeyHandler ),
		/* onKeyRepeat */ make_callback( &_gadget::gadgetKeyHandler ),
		/* onDragStart */ make_callback( &_gadget::gadgetDragStartHandler ),
		/* onDragStop */ make_callback( &_gadget::gadgetDragStopHandler ),
		/* onDragging */ make_callback( &_gadget::gadgetDraggingHandler )
	}
};


_gadget::_gadget( _gadgetType type , _optValue<_coord> posX , _optValue<_coord> posY , _optValue<_length> width , _optValue<_length> height , _bitmap&& bmp , _style&& style ) :
	padding( _padding( 0 ) )
	, minWidth( 0 )
	, minHeight( 0 )
	, x( posX )
	, y( posY )
	, style( (_style&&)style )
	, focusedChild( nullptr )
	, parent( nullptr )
	, bitmap( move( bmp ) )
	, draggedChild ( nullptr )
	, type( type )
	, state( 0 )
{
	bitmap.resize( width , height );
	autoValues.width = !width.isValid();
	autoValues.height = !height.isValid();
	autoValues.posX = !posX.isValid();
	autoValues.posY = !posY.isValid();
}


_gadget::~_gadget()
{
	_system::removeEventsOf( this );
	
	// Remove itself from parent
	this->setParent( nullptr );
	
	// Handle destructor event
	this->handleEvent( onSet );
	
	// Delete callbacks
	for( _pair<_eventType,_callback<_eventHandler>*> cb : this->eventHandlers )
	{
		if( cb.second )
			delete cb.second;
	}
	
	// Clear list and dependencies
	this->eventHandlers.clear();
	this->dependencies.sum = 0;
	
	// Remove Children
	this->removeChildren();
	this->removeEnhancedChildren();
	
	if( _system::_lastClickedGadget_ == this )
		_system::_lastClickedGadget_ = nullptr;
}


void _gadget::triggerEvent( _event event , _eventCallType callType ){
	_system::generateEvent( (_event&&)event.setDestination( this ) , callType );
}

void _gadget::redrawParents( _area&& areaToRefresh )
{
	if( !this->parent )
		return;
	
	// Crop rects to parents area
	areaToRefresh.clipToIntersect( this->parent->getAbsoluteDimensions() );
	
	if( !areaToRefresh.isEmpty() )
		this->parent->redraw( (_area&&)areaToRefresh ); // Forces std::move
}

void _gadget::redraw( _area&& areaToRefresh )
{
	if( this->parent )
	{
		this->handleEvent( _event::drawEvent( areaToRefresh ) );
		
		// Crop rects to parents area
		areaToRefresh.clipToIntersect( this->parent->getAbsoluteDimensions() );
		
		if( !areaToRefresh.isEmpty() )
			this->parent->redraw( (_area&&)areaToRefresh ); // Forces std::move
	}
	else
		this->handleEvent( _event::drawEvent( (_area&&)areaToRefresh ) ); // Forces std::move
}

void _gadget::setInternalEventHandler( _eventType type , _paramAlloc<_callback<_eventHandler>> handler )
{
	// Remove any Current Handler
	_callback<_eventHandler>* &data = this->eventHandlers[type]; // reference to pointer
	
	if( data )
		delete data; // Delete Current Event-Handler
	
	// Insert The Handler
	data = handler;
	
	// Check if we have to convert back to 'basic' eventTypes
	type = userET2eventType( type );
	
	if( isDepType( type ) ) // Check if we have to set an identity flag here
		this->addDependency( type );
}

void _gadget::removeInternalEventHandler( _eventType type ){
	// Unbind the Handler
	_eventHandlerMap::iterator data = this->eventHandlers.find( type );
	
	if( data != this->eventHandlers.end() && data->second )
	{
		delete data->second;
		this->eventHandlers.erase( data );
	}
	
	type = userET2eventType( type );
	if( isDepType( type ) )
		this->removeDependency( type );
}

bool _gadget::removeDeleteCallback( _gadget* g )
{
	// Remove focus
	if( g->parent && g->hasFocus() )
	{
		g->focused = false;
		
		if( g->parent->focusedChild == g )
			g->parent->focusedChild = nullptr;
			
		// Remove current focus
		_system::_currentFocus_ = g->parent;
	}
	
	// Trigger dependency event
	g->notifyDependentGadgets( onSet );
	
	// Remove parent from child
	g->parent = nullptr;
	
	// Delete
	delete g;
	
	return true;
}


bool _gadget::removeCallback( _gadget* g )
{
	// Remove focus
	if( g->parent && g->hasFocus() )
	{
		g->focused = false;
		
		if( g->parent->focusedChild == g )
			g->parent->focusedChild = nullptr;
			
		// Remove current focus
		_system::_currentFocus_ = g->parent;
	}
	
	// Trigger dependency event
	g->notifyDependentGadgets( onSet );
	
	// Remove parent from child
	g->parent = nullptr;
	
	return true;
}

// All about blinking
namespace{
	_timer blinkTimer = _timer( 70 , true );
}

void _gadget::blinkHandler()
{
	if( this->cntBlnk > 5 )
	{
		blinkTimer.stop();
		this->cntBlnk = 0;
		return;
	}
	if( this->cntBlnk++ % 2 )
		this->show();
	else
		this->hide();
}


void _gadget::blink()
{
	blinkTimer.stop();
	blinkTimer.setCallback( make_callback( this , &_gadget::blinkHandler ) );
	blinkTimer.start();
}


bool _gadget::hasClickRights() const
{
	if( this->hasFocus() || !this->style.canTakeFocus || !this->style.canReceiveFocus )
		return true;
	
	for( _gadget* cur = this->parent; cur ; cur = cur->parent )
	{
		if( !cur->style.canReceiveFocus || !this->style.canTakeFocus )
			return true;
	}
	return false;
}


bool _gadget::blurChild()
{
	if( this->focusedChild )
	{
		if( !this->focusedChild->style.canLooseFocus )
		{
			this->focusedChild->blink();
			return false;
		}
		
		if( !this->focusedChild->blurChild() )
			return false;
		
		this->focusedChild->focused = false;
		
		// trigger onBlur - event
		this->focusedChild->triggerEvent( onBlur );
		
		// Rest focusedChild
		this->focusedChild = nullptr;
		
		// Remove current focus
		_system::_currentFocus_ = this;
	}
	
	return true;
}

bool _gadget::focus(){
	if( this->parent )
		return this->parent->focusChild( this );
	else if( this->type == _gadgetType::screen )
		return this->style.canReceiveFocus && ( this->style.canTakeFocus || !_system::_currentFocus_ );
	return false;
}


bool _gadget::focusChild( _gadget* child )
{
	// If child
	// - is not valid or
	// - the child cannot even receive the focus or 
	// - the child is unvisible
	// return false;
	if( !child || child->isHidden() )
		return false;
	
	// Return true if the child already has focus
	if( child->hasFocus() )
		return true;
	
	// Blur the Previously focused gadget (try to)
	// Return false, if the child cannot take the focus from the currently still focused gadget
	if( focusedChild )
	{
		if( !child->style.canTakeFocus )
			return false;
		
		if( !this->blurChild() )
			return false;
	}
	
	// Return false, if the gadget cannot receive focus
	if( !child->style.canReceiveFocus )
		return false;
	
	// Check if i can get focus, if not, my child can't too
	if( !this->focus() )
		return false;
	
	// Set _currentFocus_
	_system::_currentFocus_ = child;
	
	// 'focus' the child
	child->focused = true;
	focusedChild = child;
	
	// Trigger the 'onfocus'-event
	child->triggerEvent( onFocus );
	
	// Move the child to the front of all children that it will be seen
	if( child->style.focusBringsFront )
	{
		// Move to front of children
		if( child->isEnhanced() )
		{
			// no moving neccesary?
			if( child == this->enhancedChildren.front() )
				return true;
			
			this->enhancedChildren.remove( child );
			this->enhancedChildren.push_front( child );
		}
		else
		{
			// no moving neccesary?
			if( child == this->children.front() )
				return true;
			
			this->children.remove( child );
			this->children.push_front( child );
		}
		
		// Refresh the Gadget so that it appears at the front
		child->redraw();
	}
	
	return true;
}


_callbackReturn _gadget::handleEventDefault( _event event )
{
	_s32 posInArray = _s32( event.getType() ) - 1;
	
	// Use the default EventHandler if available
	if( posInArray >= 0 && _u32(posInArray) < defaultEventHandlers.size() )
	{
		event.setGadget( this );
		return defaultEventHandlers[ posInArray ]( move(event) );
	}
	
	// If the Handler for the given event doesn't exist, return 
	return not_handled;
}


__attribute__((hot)) _callbackReturn _gadget::handleEvent( _event event , bool noDefault )
{
	const _callback<_eventHandler>* ret1 = nullptr;
	const _callback<_eventHandler>* ret2 = nullptr;
	
	_eventType eventType1 = event.getType();
	_eventType eventType2 = eventType2userET( eventType1 );
	
	auto begin = eventHandlers.begin();
	auto end = eventHandlers.end();
	while( begin != end )
	{
		if( begin->first == eventType1 )
		{
			ret2 = begin->second;
			while( begin != end ){
				if( begin->first == eventType2 ){
					ret1 = begin->second;
					break;
				}
				begin++;
			}
			break;
		}
		else if( begin->first == eventType2 )
		{
			ret1 = begin->second;
			while( begin != end ){
				if( begin->first == eventType1 ){
					ret2 = begin->second;
					break;
				}
				begin++;
			}
			break;
		}
		begin++;
	}
	
	_callbackReturn ret;
	
	// Check for Normal Event Handlers
	if( ret1 )
	{
		event.setGadget( this );
		ret = (*ret1)( event );
		if( ret == use_default )
			goto usedefault;
		if( ret == use_internal )
			goto useinternal;
		return ret;
	}
	
	useinternal:
	
	// Check for Normal Event Handlers
	if( ret2 )
	{
		event.setGadget( this );
		_callbackReturn ret = (*ret2)( event );
		if( ret == use_default || ret == use_internal )
			goto usedefault;
		return ret;
	}
	
	usedefault:
	
	if( noDefault )
		return not_handled;
	
	// If the Handler doesn't exist, return the default Handler
	return this->handleEventDefault( (_event&&)event );
}


_callbackReturn _gadget::handleEventUser( _event event )
{
	const _callback<_eventHandler>* cb = this->eventHandlers[ eventType2userET(event.getType()) ];
	
	// Check for Normal Event Handlers
	if( cb )
	{
		event.setGadget( this );
		return (*cb)( (_event&&)event );
	}
	return not_handled;
}

_callbackReturn _gadget::handleEventInternal( _event event )
{
	const _callback<_eventHandler>* cb = this->eventHandlers[ event.getType() ];
	
	// Check for Normal Event Handlers
	if( cb )
	{
		event.setGadget( this );
		return (*cb)( (_event&&)event );
	}
	return not_handled;
}


void _gadget::setXInternal( _coord val )
{
	int diff = val - this->x;
	
	if( diff )
	{
		_2s32 pos = this->getAbsolutePosition();
		this->x = val;
		
		// Refresh if there is a parent
		if( this->parent )
		{
			_length width = this->getWidth();
			_length height = this->getHeight();
			
			if( max( -diff , diff ) < width )
				this->redrawParents( _rect( pos.first , pos.second , width , height ).expandX( diff ) );
			else
				this->redrawParents(
					{
						_rect( pos.first , pos.second , width , height )
						, _rect( pos.first + diff , pos.second , width , height )
					}
				);
		}
		
		// Notify dependent gadgets
		this->notifyDependentGadgets( onMove );
	}
}


void _gadget::setYInternal( _coord val )
{
	int diff = val - this->y;
	
	if( diff )
	{
		_2s32 pos = this->getAbsolutePosition();
		this->y = val;
		
		// Refresh if there is a parent
		if( this->parent )
		{
			_length width = this->getWidth();
			_length height = this->getHeight();
			
			if( max( -diff , diff ) < height )
				this->redrawParents( _rect( pos.first , pos.second , width , height ).expandY( diff ) );
			else
				this->redrawParents(
					{
						_rect( pos.first , pos.second , width , height )
						, _rect( pos.first , pos.second + diff , width , height )
					}
				);	
		}
		// Notify dependent gadgets
		this->notifyDependentGadgets( onMove );
	}
}


void _gadget::moveRelativeInternal( _s16 dX , _s16 dY )
{
	if( !dX && !dY )
		return;
	
	_2s32 pos = this->getAbsolutePosition();
	
	this->x += dX;
	this->y += dY;
	
	// Refresh if there is a parent
	if( this->parent )
	{
		if( max( (_s16)-dX , dX ) < this->getWidth() && max( (_s16)-dY , dY ) < this->getHeight() )
			this->redrawParents( _rect( pos.first , pos.second , this->getWidth() , this->getHeight() ).expand( dX , dY ) );
		else
			this->redrawParents(
				{
					_rect( pos.first , pos.second , this->getWidth() , this->getHeight() )
					, _rect( pos.first + dX , pos.second + dY , this->getWidth() , this->getHeight() )
				}
			);
	}
	
	// Notify dependent gadgets
	this->notifyDependentGadgets( onMove );
}


void _gadget::enhanceToParent( _gadget* val )
{
	if( this->parent == val )
		return;		
	
	if( val )
	{
		if( this->parent )
			this->parent->removeChild( this );
		val->addEnhancedChild( this );
	}
	else
		this->parent->removeChild( this );
}


void _gadget::setParent( _gadget* val )
{
	if( this->parent == val )
		return;
	
	if( val )
	{
		if( this->parent )
			this->parent->removeChild( this );
		val->addChild( this );
	}
	else
		this->parent->removeChild( this );
}


void _gadget::removeChildren( bool remove )
{
	if( this->children.empty() )
		return;
	
	if( remove )
		this->children.remove_if( _gadget::removeDeleteCallback );
	else
		this->children.remove_if( _gadget::removeCallback );
	
	//! Signalize there are no children left!
	this->redraw();
}


void _gadget::removeEnhancedChildren( bool remove )
{
	if( this->enhancedChildren.empty() )
		return;
	
	if( remove )
		this->enhancedChildren.remove_if( _gadget::removeDeleteCallback );
	else
		this->enhancedChildren.remove_if( _gadget::removeCallback );
	
	//! Signalize there are no children left!
	this->redraw();
}


void _gadget::removeChild( _gadget* child )
{
	if( !child )
		return;
		
	// Erase it on my bitmap
	_rect absDim = child->getAbsoluteDimensions();
	
	// Remove focus
	if( child->hasFocus() )
	{
		child->focused = false;
		
		if( this->focusedChild == child )
			this->focusedChild = nullptr;
		
		// Remove current focus
		_system::_currentFocus_ = this;
	}
	
	//! Temp...
	_gadget* subcedent = child->getSubcedentChild();
	_gadget* precedent = child->getPrecedentChild();
	
	// Erase the Connection on both sides
	if( child->isEnhanced() )
		this->enhancedChildren.remove( child );
	else
		this->children.remove( child );
	
	// Remove parent of child
	child->parent = nullptr;
	
	// Make it appear deleted
	this->redraw( absDim );
	
	// Notify dependent adjacents
	child->notifyDependentAdjacents( onSet , _dependencyParam() , precedent , subcedent );
	// Notify itself that it does not have a parent anymore
	child->notifyDependentSelf( onParentSet , _dependencyParam().setSource(this) );
	// Notify parent (this gadget)
	this->notifyDependentSelf( onChildSet , _dependencyParam().setSource(child) );
}

void _gadget::addChild( _gadget* child , bool pushBack )
{	
	if( !child || child->parent == this )
		return;
	
	// Add it!
	if( pushBack )
		this->children.push_back( child );
	else
		this->children.push_front( child );
	
	// Reset style-object
	child->resetState();
	
	// Set Parent
	child->parent = this;
	
	// Paint it on my bmp
	child->redraw();
	
	// Notify dependent gadgets
	child->notifyDependentParent( onSet );
	child->notifyDependentAdjacents( onSet );
	
	// Notify itself that it has a new parent
	if( child->isDependentOf( onParentSet ) )
		child->notifyDependentSelf( onParentSet , _dependencyParam().setSource(this) );
}


void _gadget::addEnhancedChild( _gadget* child , bool pushBack )
{
	if( !child || child->parent == this )
		return;
	
	// Add it!
	if( pushBack )
		this->enhancedChildren.push_back( child );
	else
		this->enhancedChildren.push_front( child );
	
	// Reset style-object
	child->resetState();
	child->enhanced = true;
	
	// Set Parent
	child->parent = this;
	
	//! Paint it on my bmp
	child->redraw();
	
	// Notify dependent gadgets
	child->notifyDependentGadgets( onSet , false );
	child->notifyDependentSelf( onParentSet , _dependencyParam().setSource(this) );
}


_2s32 _gadget::getAbsolutePosition() const
{
	_gadget* p = this->parent;
	
	if( p )
	{
		_2s32 val = p->getAbsolutePosition(); // Rekursion
		
		if( this->isEnhanced() )
			return _2s32( val.first + this->x , val.second + this->y );
		
		return _2s32( val.first + this->x + p->padLeft , val.second + this->y + p->padTop );
	}
	return _2s32( this->x , this->y );
}


void _gadget::setDimensionsInternal( _rect rc )
{
	_2s32 pos = this->getAbsolutePosition();
	_2s32 size = this->getSize();
	
	// Respect Fixed width/height of the gadget
	if( !this->isResizeableX() )
		rc.width = this->getWidth();
	if( !this->isResizeableY() )
		rc.height = this->getHeight();
	
	// Limit
	rc.width = max( rc.width , this->getMinWidth() );
	rc.height = max( rc.height , this->getMinHeight() );
	
	// Apply Position
	this->x = rc.x;
	this->y = rc.y;
	
	// Compute new position
	_2s32 newPos = this->getAbsolutePosition();
	_2s32 newSize = _2s32( rc.width , rc.height );
	
	if( size != newSize )
	{
		this->bitmap.resize( rc.width , rc.height );
		
		// Notify dependent gadgets
		this->notifyDependentGadgets( onResize );
		if( pos != newPos )
			this->notifyDependentGadgets( onMove );
		
		if( !this->parent )
			this->redraw();
		else
			this->redraw( _rect(pos, size).combine( _rect(newPos, newSize) ) );
	}
	else if( pos != newPos )
	{
		this->redrawParents( _rect( pos , size ).combine( _rect( newPos , size ) ) );
		
		// Notify dependent gadgets
		this->notifyDependentGadgets( onMove );
	}
}


void _gadget::setHeightInternal( _length val )
{
	if( !this->isResizeableY() )
		return;
	
	val = max( val , this->getMinHeight() );
	
	// Check if something to do
	if( val == this->getHeight() )
		return;
	
	// Save old dimensions
	_rect dim = this->getAbsoluteDimensions();
	dim.height = max( val , dim.height );
	
	// Effectively set width
	this->bitmap.setHeight( val );
	
	// Notify dependent gadgets
	this->notifyDependentGadgets( onResize );
	
	// Refresh...
	this->redraw( (_rect&&)dim );
}


void _gadget::setWidthInternal( _length val )
{
	if( !this->isResizeableX() )
		return;
	
	val = max( val , this->getMinWidth() );
	
	// Check if something to do
	if( val == this->getWidth() )
		return;
	
	// Save old dimensions
	_rect dim = this->getAbsoluteDimensions();
	dim.width = max( val , dim.width );
	
	// Effectively set width
	this->bitmap.setWidth( val );
	
	// Notify dependent gadgets
	this->notifyDependentGadgets( onResize );
	
	// Refresh...
	this->redraw( dim );
}


void _gadget::setBitmap( _bitmap bmp )
{
	_2s32 pos = this->getAbsolutePosition();
	_2s32 size = this->getSize();
	
	_length newWidth = bmp.getWidth();
	_length newHeight = bmp.getHeight();
	
	
	// Respect Fixed width/height of the gadget and size limits
	if( !this->isResizeableX() )
		newWidth = this->getWidth();
	else
		newWidth = max( newWidth , this->getMinWidth() );
	
	if( !this->isResizeableY() )
		newHeight = this->getHeight();
	else
		newHeight = max( newHeight , this->getMinHeight() );
	
	
	// Resize bitmap to applyable size
	bmp.resize( newWidth , newHeight );
	
	this->bitmap = move(bmp);
	
	// Compute area to redraw
	_2s32 refreshSize = _2s32( max( newWidth , (_length)size.first ) , max( newHeight , (_length)size.second ) );
	
	// Redraw!
	this->redraw( _rect( pos , refreshSize ) );
	
	
	// Notify dependent gadgets
	if( newWidth != this->getWidth() || newHeight != this->getHeight() )
		this->notifyDependentGadgets( onResize );
}


void _gadget::setSizeInternal( _length width , _length height )
{
	_2s32 size = this->getSize();
	
	// Respect Fixed width/height of the gadget and size limits
	if( !this->isResizeableX() )
		width = this->getWidth();
	else
		width = max( width , this->getMinWidth() );
	
	if( !this->isResizeableY() )
		height = this->getHeight();
	else
		height = max( height , this->getMinHeight() );
	
	if( width == this->getWidth() && height == this->getHeight() )
		return;
	
	// Compute area to redraw
	_2s32 refreshSize = _2s32( max( width , (_length)size.first ) , max( height , (_length)size.second ) );
	_2s32 refreshPos = this->getAbsolutePosition();
	
	// Refresh
	this->bitmap.resize( width , height );
	
	// Notify dependent gadgets
	this->notifyDependentGadgets( onResize );
	
	// Refresh
	this->redraw( _rect( refreshPos , refreshSize ) );
}


_callbackReturn _gadget::gadgetRefreshHandler( _event event )
{
	_codeAnalyzer analyzer {"_gadget::gadgetRefreshHandler"};
	
	_gadget* that = event.getGadget();
	
	// Break up, if there are no children to paint!
	if( that->children.empty() && that->enhancedChildren.empty() )
		return handled;
	
	// Receive Bitmap-Port
	_bitmapPort bP = that->getBitmapPort( event );
	
	// Draw All enhanced Children
	
	int numEnhancedChildren = that->children.empty() ? that->enhancedChildren.size() : 0;
	int cnt1 = 0;
	
	for( _gadget* gadget : that->enhancedChildren )
	{
		if( gadget->isHidden() )
			continue;
		
		_rect dim = gadget->getDimensions();
		
		// Copy...
		bP.copyTransparent( dim.x , dim.y , gadget->getBitmap() );
		
		if( ++cnt1 == numEnhancedChildren )
			break;
		
		// Reduce Painting Area
		bP.clippingRects.reduce( dim );
	}
	
	// Break up?
	if( that->children.empty() )
		return handled;
	
	// Apply the parents padding to the drawing area
	bP.clippingRects.clipToIntersect( that->getSizeRect().applyPadding( that->getPadding() ) );
	
	// Store padding in termporaries
	_length padLeft = that->padLeft;
	_length padTop = that->padTop;
	
	
	// Draw all other children
	
	int numChildren = that->children.size();
	int cnt2 = 0;
	
	for( _gadget* gadget : that->children )
	{
		if( gadget->isHidden() )
			continue;
		
		_rect dim = gadget->getDimensions().toRelative( -padLeft , -padTop );
		
		// Copy...
		bP.copyTransparent( dim.x , dim.y , gadget->getBitmap() );
		
		if( ++cnt2 == numChildren )
			break;
		
		// Reduce Painting Area
		bP.clippingRects.reduce( dim );
	}
	
	return handled;
}


_gadget* _gadget::getGadgetOfMouseDown( _coord posX , _coord posY )
{
	_coord posPadX = posX - this->padLeft;
	_coord posPadY = posY - this->padTop;
	
	for( _gadget* gadget : this->enhancedChildren )
	{
		// Check if event position was inside this Gadget's Area
		if( gadget->isHidden() )
			continue;
		
		if( gadget->getDimensions().contains( posX , posY ) )
			return gadget;
	}
	
	for( _gadget* gadget : this->children )
	{
		// Check if event position was inside this Gadget's Area
		if( gadget->isHidden() )
			continue;
		
		if( gadget->getDimensions().contains( posPadX , posPadY ) )
			return gadget;
	}
	
	return nullptr;
}


_callbackReturn	_gadget::gadgetKeyHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	// Forward key-event to parent
	if( that->parent )
		that->parent->handleEvent( move(event) );
	
	return not_handled;
}


_callbackReturn _gadget::gadgetMouseHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	// Temp...
	_gadget* mouseContain = that->getGadgetOfMouseDown( event.getPosX() , event.getPosY() );
	
	if( mouseContain )
	{
		// Rewrite Destination
		event.setDestination( mouseContain );
		
		// Absolute Position to Relative Position
		event.posX -= mouseContain->getX();
		event.posY -= mouseContain->getY();
		
		if( !mouseContain->isEnhanced() )
		{
			event.posX -= that->padLeft;
			event.posY -= that->padTop;
		}
		
		// Trigger the Event if the gadget is now focused or if it never will have any focus,
		if( mouseContain->isClickable() )
		{
			// Check for children of the 'mouseContain'
			_callbackReturn ret = mouseContain->handleEventDefault( event );
			
			// Abort if there's nothing to do about the 'mouseCoantain' but about its children
			if( ret != not_handled )
				return ret;
			
			// Try to focus the child if its not focused yet
			if( event == onMouseDown )
				that->focusChild( mouseContain );
			
			// Independent of whether the gadget has focus or not
			if( event == onMouseUp && mouseContain->pressed )
			{
				mouseContain->pressed = false; // adjust state
				mouseContain->triggerEvent( onMouseLeave );
			}
			
			if( mouseContain->hasClickRights() )
			{
				if( event == onMouseDown ){
					mouseContain->pressed = true;
					mouseContain->triggerEvent( onMouseEnter );
				}
				
				// Call the user-registered event-handler
				mouseContain->handleEvent( (_event&&)event , true );
			}
			
			return handled;
		}
	}
	// If no Gadget received the Mousedown, blur the Focussed Child
	else if( event == onMouseDown )
		that->blurChild();
	
	_system::_lastClickedGadget_ = that;
	
	return not_handled;
}


_callbackReturn _gadget::gadgetDraggingHandler( _event event )
{
	// Temp...
	_gadget* that = event.getGadget();
	_gadget*& draggedChild = that->draggedChild; // Shortcut
	
	// 'dragging'-events without an object doesn't make sence
	if( !draggedChild )
		return prevent_default;
	
	// Make touch positions relative to parent
	event.posX -= that->getX();
	event.posY -= that->getY();
	
	// if the child is not enhanced
	if( !draggedChild->isEnhanced() ){
		event.posX -= that->padLeft;
		event.posY -= that->padTop;
	}
	
	if( // Check if we have to fire an 'onMouseLeave'-event
		!draggedChild->isDraggable() // Do not fire if 'draggedChild' is draggable, because it would look strange on some gadgets (e.g. scrollbutton)
		&& draggedChild->pressed
		&& !draggedChild->getDimensions().contains( event.getPosX() , event.getPosY() )
	){
		draggedChild->pressed = false;
		draggedChild->triggerEvent( onMouseLeave );
	}
	
	// Forward 'dragging'-event to child
	if( draggedChild->dragged )
	{
		// Rewrite Destination and retrigger the Event
		if( draggedChild->handleEvent( (_event&&)event ) == prevent_default )
		{
			draggedChild->dragged = false; // If the dragged gadget no longer wants to be dragged
			return prevent_default; // Force the upper gadget also to stop dragging
		}
	}
	else
		return draggedChild->handleEventDefault( (_event&&)event );
	
	return handled;
}


_callbackReturn _gadget::gadgetDragStopHandler( _event event )
{
	// Temp...
	_gadget* that = event.getGadget();
	_gadget*& draggedChild = that->draggedChild; // Shortcut
	
	// 'dragStop'-events without an object doesn't make sence
	if( !draggedChild )
		return prevent_default;
	
	// Check if the child was actually dragged, if not-> don't fire an 'onDragStop'-Event!
	if( draggedChild->dragged )
	{
		// Make touch positions relative to parent
		event.posX -= that->getX();
		event.posY -= that->getY();
		
		// if the child is not enhanced
		if( !draggedChild->isEnhanced() ){
			event.posX -= that->padLeft;
			event.posY -= that->padTop;
		}
		
		// Rewrite Destination and trigger the Event
		draggedChild->handleEvent( (_event&&)event );
		
		// Update _style
		draggedChild->dragged = false;
	}
	else
		draggedChild->handleEventDefault( (_event&&)event ); // Forward the dragStop-event to the children
	
	if( draggedChild->pressed ){
		draggedChild->pressed = false;
		draggedChild->triggerEvent( onMouseLeave );
	}
	
	// No Gadget will receive Events anymore (reset)
	draggedChild = nullptr;
	
	return handled;
}


_callbackReturn _gadget::gadgetDragStartHandler( _event event )
{
	// Temp...
	_gadget* that = event.getGadget();
	
	// Temp...
	_coord posX = event.getPosX();
	_coord posY = event.getPosY();
	
	// Start Dragging
	_gadget* mouseContain = that->getGadgetOfMouseDown( posX , posY );
	
	if( !mouseContain )
		return not_handled;
	
	// Rewrite Destination
	event.setDestination( mouseContain );
	
	// Make the touch position relative to the Child
	event.posX -= mouseContain->getX();
	event.posY -= mouseContain->getY();
	
	// if the child is not enhanced
	if( !mouseContain->isEnhanced() )
	{
		event.posX -= that->padLeft;
		event.posY -= that->padTop;
	}
	
	// Set Gadget, which receives all other drag Events until dragStop is called
	that->draggedChild = mouseContain;
	
	_callbackReturn ret = mouseContain->handleEventDefault( event );
	
	// Mark the gadget as 'dragged' (but only if no child wants to be dragged -> check that first)
	if( ret == not_handled )
	{
		if(
			mouseContain->hasClickRights()
			&& mouseContain->isDraggable()
			&& mouseContain->isClickable()
			&& mouseContain->handleEvent( (_event&&)event , true ) != prevent_default
		)
			mouseContain->dragged = true;
		else
			return not_handled;
	}
	
	return handled;
}


_gadget* _gadget::getSubcedentChild( bool skipHidden )
{
	if( this->parent )
	{
		_gadgetList& ch = this->isEnhanced() ? this->parent->enhancedChildren : this->parent->children;
		
		auto it = find( ch.begin() , ch.end() , this );
		
		if( it == ch.end() )
			return nullptr;
		
		while( ++it != ch.end() )
		{
			if( !skipHidden || !(*it)->isHidden() )
				return *it;
			it++;
		}
	}
	return nullptr;
}

_gadget* _gadget::getPrecedentChild( bool skipHidden )
{
	if( this->parent )
	{
		_gadgetList& ch = this->isEnhanced() ? this->parent->enhancedChildren : this->parent->children;
		
		auto it = find( ch.rbegin() , ch.rend() , this );
		
		if( it == ch.rend() )
			return nullptr;
		
		while( ++it != ch.rend() )
		{
			if( !skipHidden || !(*it)->isHidden() )
				return *it;
		}
	}
	return nullptr;
}


void _gadget::notifyDependentSelf( _eventType change , _dependencyParam param ){
	this->handleEvent( _event::dependencyEvent( change , param ) );
}


void _gadget::notifyDependentParent( _eventType change , _dependencyParam param , _optValue<_gadget*> parent )
{
	_gadget* g = parent.isValid() ? (_gadget*)parent : this->parent;
	
	if( !g )
		return;
	
	_eventType newType = depType2childType( change );
	
	// Notifying Parent...
	if( g->isDependentOf( newType ) )
		g->handleEvent( _event::dependencyEvent( newType , param.setSource( this ) ) );
}


void _gadget::notifyDependentChildren( _eventType type , _dependencyParam param )
{
	_eventType newType = depType2parentType( type );
	
	// Children
	for( _gadget* g : this->children )
		if( g->isDependentOf( newType ) )
			g->handleEvent( _event::dependencyEvent( newType , param.setSource( this ) ) );
	
	// Enhanced Children
	for( _gadget* g : this->enhancedChildren )
		if( g->isDependentOf( newType ) )
			g->handleEvent( _event::dependencyEvent( newType , param.setSource( this ) ) );
}


void _gadget::notifyDependentAdjacents( _eventType type , _dependencyParam param , _optValue<_gadget*> pre , _optValue<_gadget*> post )
{
	_gadget* g1 = post.isValid() ? (_gadget*)post : this->getSubcedentChild();
	_gadget* g2 = pre.isValid() ? (_gadget*)pre : this->getPrecedentChild();
	
	// Subcedent child
	if( g1 ){
		_eventType newType = depType2preType( type );
		
		if( g1->isDependentOf( newType ) )
			g1->handleEvent( _event::dependencyEvent( newType , param.setSource( this ) ) );
	}
	
	// Precedent child
	if( g2 ){
		_eventType newType = depType2postType( type );
		
		if( g2->isDependentOf( newType ) )
			g2->handleEvent( _event::dependencyEvent( newType , param.setSource( this ) ) );
	}
}