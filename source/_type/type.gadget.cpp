#include "_type/type.gadget.h"
#include "_type/type.gadgetScreen.h"
#include "_type/type.system.h"

_map<_eventType,_callback*> _gadget::defaultEventHandlers = {
	{ mouseDown , new _staticCallback( &_gadget::gadgetMouseHandler ) },
	{ mouseRepeat , new _staticCallback( &_gadget::gadgetMouseHandler ) },
	{ mouseUp , new _staticCallback( &_gadget::gadgetMouseHandler ) },
	{ mouseClick , new _staticCallback( &_gadget::gadgetMouseHandler ) },
	{ mouseDoubleClick , new _staticCallback( &_gadget::gadgetMouseHandler ) },
	{ refresh , new _staticCallback( &_gadget::gadgetRefreshHandler ) },
	{ dragStart , new _staticCallback( &_gadget::gadgetDragHandler ) },
	{ dragStop , new _staticCallback( &_gadget::gadgetDragHandler ) },
	{ dragging , new _staticCallback( &_gadget::gadgetDragHandler ) },
	{ keyDown , new _staticCallback( &_gadget::gadgetKeyHandler ) },
	{ keyUp , new _staticCallback( &_gadget::gadgetKeyHandler ) },
	{ keyClick , new _staticCallback( &_gadget::gadgetKeyHandler ) },
	{ keyRepeat , new _staticCallback( &_gadget::gadgetKeyHandler ) },
};


_gadget::_gadget( _gadgetType type , int width , int height , int posX , int posY , _style style , bool doNotAllocateBitmap ) :
	padding( _padding( 0 ) )
	, dimensions( _rect( posX , posY , max( 1 , width )
	, max( 1 , height ) ) )
	, style( style )
	, focusedChild( nullptr )
	, parent( nullptr )
	, dragTemp ( nullptr )
	, type( type )
	, state( 0 )
{
	if( !doNotAllocateBitmap )
		this->bitmap = _bitmap( this->getWidth() , this->getHeight() );
}


// Delegating Constructors!!!! C++0x I love you!
_gadget::_gadget( int width , int height , int posX , int posY , _style style , bool doNotAllocateBitmap )
	: _gadget( _gadgetType::_plain , width , height , posX , posY , style , doNotAllocateBitmap )
{ }


_gadget::~_gadget()
{	
	// Unbind event Handler
	for( const _pair<_eventType,_callback*>& data : this->eventHandlers )
	{
		if( data.second )
			delete data.second;
	}
	
	// Remove Children
	this->removeChildren();
	this->removeEnhancedChildren();
	
	if( this->parent != nullptr )
		this->parent->removeChild( this );
	
	_system::removeEventsOf( this );
}


void _gadget::triggerEvent( _event event )
{
	event.setDestination( this );
	_system::generateEvent( event );
}


void _gadget::bubbleEvent( _event event , bool includeThis )
{
	if( event.getType() == refresh )
	{
		this->bubbleRefresh( includeThis , event );
		return;
	}
	if( includeThis )
		this->handleEvent( event );
	if( this->parent != nullptr )
		this->parent->bubbleEvent( event , true );
}


void _gadget::populateEvent( _event event ){
	_system::generateEvent( event );
}


void _gadget::bubbleRefresh( bool includeThis , _event event )
{
	// Event not generated yet
	if( !event.hasClippingRects() )
		event = _event::refreshEvent( { this->getAbsoluteDimensions() } );
	
	// Bubble!
	if( this->parent != nullptr )
	{
		if( includeThis )
			this->handleEvent( event );
		
		_rect dim = this->parent->getAbsoluteDimensions();
		
		if( dim.intersectsWith( event.getDamagedRects() ) )
			this->parent->bubbleRefresh( true , (_event&&)event ); // Forces std::move
	}
	else if( includeThis )
		this->handleEvent( (_event&&)event );
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
		_system::_currentFocus_ = nullptr;
	}
	
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
		_system::_currentFocus_ = nullptr;
	}
	
	g->parent = nullptr;
	
	return true;
}


void _gadget::blinkHandler()
{
	if( this->style.unused > 5 )
	{
		_system::terminateTimer( _classCallback( this , &_gadget::blinkHandler ) );
		this->style.unused = 0;
		return;
	}
	if( this->style.unused++ % 2 )
		this->show();
	else
		this->hide();
}


void _gadget::blink()
{
	_system::terminateTimer( _classCallback( this , &_gadget::blinkHandler ) );
	_system::executeTimer( new _classCallback( this , &_gadget::blinkHandler ) , 70 , true );
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
		
		_gadget* t = focusedChild;
		
		this->focusedChild = nullptr;
		
		// trigger onBlur - event
		t->handleEvent( onBlur );
		
		// Remove current focus
		_system::_currentFocus_ = nullptr;
	}
	return true;
}


bool _gadget::focusChild( _gadget* child )
{
	// If child
	// - is not valid or
	// - the child cannot even receive the focus or 
	// - the child is unvisible
	// return false;
	if( !child || child->isInvisible() )
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
	
	// Check if i can get focus, if not my child can't get focus either
	if( !this->focus() )
		return false;
	
	// Set _currentFocus_
	_system::_currentFocus_ = child;
	
	// 'focus' the child
	child->focused = true;
	focusedChild = child;
	
	// Trigger the 'onfocus'-event
	child->handleEvent( onFocus );
	
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
	child->bubbleRefresh();
	
	return true;
}


void _gadget::registerEventHandler( _eventType type , _callback* handler )
{
	// Remove any Current Handler
	_callback* &data = this->eventHandlers[type]; // reference to pointer
	
	if( data )
	{
		// Delete Current Event-Handler
		delete data;
	}
	
	// Insert The Handler
	data = handler;
}


void _gadget::unregisterEventHandler( _eventType type ){
	// Unbind the Handler
	_map<_eventType,_callback*>::iterator data = this->eventHandlers.find( type );
	
	if( data != this->eventHandlers.end() )
	{
		delete data->second;
		
		this->eventHandlers.erase( data );
	}
}


_callbackReturn _gadget::handleEventDefault( _event&& event )
{
	// Use the default EventHandler if available
	if( _gadget::defaultEventHandlers.count( event.getType() ) )
	{
		event.setGadget( this );
		return (*defaultEventHandlers[ event.getType() ])( event );
	}
	
	// If the Handler for the given event doesn't exist, return 
	return not_handled;
}


_callbackReturn _gadget::handleEvent( _event&& event )
{
	// Check for Normal Event Handlers
	if( this->eventHandlers.count( event.getType() ) )
	{
		event.setGadget( this );
		_callbackReturn ret = (*eventHandlers[ event.getType() ])( event );
		if( ret != use_default )
			return ret;
	}
	
	// If the Handler doesn't exist, return the default Handler
	return this->handleEventDefault( event );
}


void _gadget::setX( _coord val )
{
	if( val == this->x )
		return;
	_rect dim = this->getAbsoluteDimensions();
	this->x = val;
	this->bubbleRefresh( false , _event::refreshEvent( dim.combine( this->getAbsoluteDimensions() ) ) );
}


void _gadget::setY( _coord val )
{
	if( val == this->y )
		return;
	_rect dim = this->getAbsoluteDimensions();
	this->y = val;
	this->bubbleRefresh( false , _event::refreshEvent( dim.combine( this->getAbsoluteDimensions() ) ) );
}


void _gadget::moveRelative( _s16 dX , _s16 dY )
{
	if( !dX && !dY )
		return;
	_rect dim = this->getAbsoluteDimensions();
	this->x += dX;
	this->y += dY;
	this->bubbleRefresh( false , _event::refreshEvent( dim.combine( this->getAbsoluteDimensions() ) ) );
}


void _gadget::moveTo( _coord x , _coord y )
{
	if( x == this->x && y == this->y )
		return;
	_rect dim = this->getAbsoluteDimensions();
	this->x = x;
	this->y = y;
	//if( this->parent )
	//	this->parent->bitmap->move( dim.x , dim.y , this->x , this->y , this->width , this->height );
	this->bubbleRefresh( false , _event::refreshEvent( dim.combine( this->getAbsoluteDimensions() ) ) );
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
		this->children.remove_if( this->removeDeleteCallback );
	else
		this->children.remove_if( this->removeCallback );
	
	//! Signalize there are no children left!
	this->bubbleRefresh( true );
}


void _gadget::removeEnhancedChildren( bool remove )
{
	if( this->enhancedChildren.empty() )
		return;
	
	if( remove )
		this->enhancedChildren.remove_if( this->removeDeleteCallback );
	else
		this->enhancedChildren.remove_if( this->removeCallback );
	
	//! Signalize there are no children left!
	this->bubbleRefresh( true );
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
		_system::_currentFocus_ = nullptr;
	}
	
	if( child->isEnhanced() )
		this->enhancedChildren.remove( child );
	else
		this->children.remove( child );
	
	this->bubbleRefresh( true , _event::refreshEvent( { absDim } ) );
	
	// Erase the Connection on both sides
	child->parent = nullptr;
}


void _gadget::addChild( _gadget* child )
{	
	if( !child )
		return;
	
	// Add it!
	this->children.push_front( child );
	
	// Adjust style-object (kind of reset)
	child->focused = false;
	child->enhanced = false;
	child->hidden = false;
	child->parent = this;
	
	//! Paint it on my bmp
	child->bubbleRefresh( true );
}


void _gadget::addEnhancedChild( _gadget* child )
{
	if( !child )
		return;
	
	// Add it!
	this->enhancedChildren.push_front( child );
	
	// Adjust style-object (kind of reset)
	child->focused = false;
	child->enhanced = true;
	child->hidden = false;
	child->parent = this;
	
	//! Paint it on my bmp
	child->bubbleRefresh( true );
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


void _gadget::setDimensions( _rect rc )
{
	if( !rc.isValid() )
		return;
	
	_rect absDim = this->getAbsoluteDimensions();
	
	// Respect Fixed width/height of the gadget
	if( !this->isResizeableX() )
		rc.width = this->width;
	if( !this->isResizeableY() )
		rc.height = this->height;
	
	this->dimensions = rc;
	_rect newAbsDim = this->getAbsoluteDimensions();
	
	if( newAbsDim.width != absDim.width || newAbsDim.height != absDim.height )
	{
		this->bitmap.resize( rc.width , rc.height );
		this->handleEvent( onResize );
		this->bubbleEvent( _event::refreshEvent( absDim.combine( newAbsDim ) ) , true );
	}
	else if( newAbsDim.x != absDim.x || newAbsDim.y != absDim.y )
		// Delete the parts that originally were gadget, but became damaged
		this->bubbleEvent( _event::refreshEvent( absDim.combine( newAbsDim ) ) );
}


void _gadget::setHeight( _length val )
{
	if( int(val) < 1 )
		val = 1;
	
	if( !this->isResizeableY() || val == this->height )
		return;
	
	_rect dim = this->getAbsoluteDimensions();
	this->height = val;
	this->bitmap.setHeight( val );
	
	this->handleEvent( onResize );
	
	// Delete the parts that originally were gadget, but became damaged
	this->bubbleEvent( _event::refreshEvent( dim.combine( this->getAbsoluteDimensions() ) ) , true );
}


void _gadget::setWidth( _length val )
{
	if( int(val) < 1 )
		val = 1;
	
	if( !this->isResizeableX() || val == this->width )
		return;
	
	_rect dim = this->getAbsoluteDimensions();
	this->width = val;
	this->bitmap.setWidth( val );
	
	this->handleEvent( onResize );
	
	// Delete the parts that originally were gadget, but became damaged
	this->bubbleEvent( _event::refreshEvent( dim.combine( this->getAbsoluteDimensions() ) ) , true );
}


void _gadget::minimize()
{
	if( !this->minimized && this->style.minimizeable )
	{
		// Blur
		this->blur();
		
		this->minimized = true;
		this->bubbleRefresh();
		this->triggerEvent( onMinimize );
	}
}


void _gadget::unMaximize()
{
	if( this->maximized )
	{
		this->maximized = false;
		
		// Set back the old dimensions
		this->setDimensions( this->normalDimensions );
		this->triggerEvent( onUnMaximize );
	}
}


void _gadget::restore()
{
	if( this->minimized )
	{
		this->minimized = false;
		this->bubbleRefresh();
		this->triggerEvent( onRestore );
	}
}


void _gadget::maximize()
{
	if( this->maximized || !this->isResizeable() )
		return;
	
	_gadgetScreen* screen = this->getScreen();
	
	// Fail
	if( screen == nullptr )
		return;
	
	// Fetch maximized Dimensions from the _screen
	_rect maxDim = screen->getMaximizedDimensions();
	
	if( maxDim.isValid() )
	{
		this->maximized = true;
		this->normalDimensions = this->dimensions;
		
		// Maximizing
		this->setDimensions( maxDim );
		this->triggerEvent( onMaximize );
	}
}


_callbackReturn _gadget::gadgetRefreshHandler( _event event )
{
	//startTimer( reinterpret_cast<void*>(&_gadget::gadgetRefreshHandler) );
	_gadget* that = event.getGadget();
	
	// Receive Bitmap-Port
	_bitmapPort bP = that->getBitmapPort();
	
	// Receive Areas
	_area damagedRects;
	_area damagedEnhancedRects;
	_rect areaAvailable = { 0 , 0 , that->width , that->height };
	areaAvailable.applyPadding( that->getPadding() );
	
	if( event.hasClippingRects() )
	{
		damagedRects = event.getDamagedRects();
		damagedRects.toRelative( that->getAbsoluteX() , that->getAbsoluteY() );
		damagedEnhancedRects = damagedRects;
		damagedRects.clipToIntersect( areaAvailable );
	}
	else
	{
		damagedRects.add( areaAvailable );
		damagedEnhancedRects.clearRects();
		damagedEnhancedRects.add( _rect( 0 , 0 , that->width , that->height ) );
	}
	
	_length padLeft = that->padLeft;
	_length padTop = that->padTop;
	
	for( _gadget* gadget : that->children )
	{
		if( gadget->isInvisible() )
			continue;
		
		_rect dim = gadget->getDimensions().relativeVersion( -padLeft , -padTop );
		
		// Reset clipping Rects
		bP.deleteClippingRects();
		
		// Has the Gadget special Privilegs event.g. it can draw on the Parents reserved areas?
		bP.addClippingRects( damagedRects );
		
		// Copy...
		bP.copyTransparent( dim.x , dim.y , gadget->getBitmap() );
		
		// Reduce Painting Area
		damagedRects.reduce( dim );
	}
	
	for( _gadget* gadget : that->enhancedChildren )
	{
		if( gadget->isInvisible() )
			continue;
		
		_rect dim = gadget->getDimensions();
		
		// Reset clipping Rects
		bP.deleteClippingRects();
		
		// Special Area for Enhanced Gadgets
		bP.addClippingRects( damagedEnhancedRects );
		
		// Copy...
		bP.copyTransparent( dim.x , dim.y , gadget->getBitmap() );
		
		// Reduce Painting Area
		damagedEnhancedRects.reduce( dim );
	}
	
	// If this Refresh-Event wasn't auto-generated, refresh my parents
	if( !event.isBubblePrevented() )
		that->bubbleRefresh();
	
	//stopTimer( reinterpret_cast<void*>(&_gadget::gadgetRefreshHandler) );
	
	return handled;
}


_gadget* _gadget::getGadgetOfMouseDown( _coord posX , _coord posY )
{
	_coord posPadX = posX - this->padLeft;
	_coord posPadY = posY - this->padTop;
	
	for( _gadget* gadget : this->enhancedChildren )
	{
		// Check if event position was inside this Gadget's Area
		if( gadget->isInvisible() )
			continue;
		
		if( gadget->getDimensions().contains( posX , posY ) )
			return gadget;
	}
	
	for( _gadget* gadget : this->children )
	{
		// Check if event position was inside this Gadget's Area
		if( gadget->isInvisible() )
			continue;
		
		if( gadget->getDimensions().contains( posPadX , posPadY ) )
			return gadget;
	}
	
	return nullptr;
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
		if( mouseContain->isEnhanced() )
		{
			event.posX -= mouseContain->getX();
			event.posY -= mouseContain->getY();
		}
		else
		{
			event.posX -= that->padLeft + mouseContain->getX();
			event.posY -= that->padTop + mouseContain->getY();
		}
		
		// It doesn't make sense to focus a child of some _gadget that can't be focused
		if( event.getType() == mouseDown )
		{
			// Update _lastClickedGadget_ so that specialities like hasSmallDragTrig work! See _gadgetScreen::processTouch()
			_system::_lastClickedGadget_ = mouseContain;
			
			// Try to focus the child
			that->focusChild( mouseContain );
		}
		
		// Trigger the Event if the gadget is now focused or if it never will have any focus,
		// because then it wouldn't make any sence to forward the event!
		if( mouseContain->hasClickRights() )
		{	
			if( event.getType() == mouseDown )
			{
				mouseContain->pressed = true;
				mouseContain->triggerEvent( onMouseEnter );
			}
			else if( event.getType() == mouseUp && mouseContain->pressed )
			{
				mouseContain->pressed = false; // adjust state
				mouseContain->triggerEvent( onMouseLeave );
			}
			return mouseContain->handleEvent( event );
		}
		
		return not_handled;
	}
	else
		_system::_lastClickedGadget_ = that;
	
	// If no Gadget received the Mousedown, blur the Focussed Child
	if( event.getType() == mouseDown )
		that->blurChild();
	
	return not_handled;
}


_callbackReturn _gadget::gadgetDragHandler( _event event )
{
	// Temp...
	_gadget* that = event.getGadget();
	_gadget*& dragTemp = that->dragTemp;
	
	// Temp...
	_coord posX = event.getPosX();
	_coord posY = event.getPosY();
	
	// Start Dragging
	if( event.getType() == dragStart )
	{
		// Temp...
		_gadget* mouseContain = that->getGadgetOfMouseDown( posX , posY );
		
		if( !mouseContain )
			return not_handled;
		
		//! Trigger the Event if the gadget is now focused or if it never will have any focus,
		//! because otherwise it wouldn't make any sence to forward the event!
		if( !mouseContain->hasClickRights() )
			return not_handled;
		
		// Rewrite Destination
		event.setDestination( mouseContain );
		
		// Absolute Position to Relative Position
		if( mouseContain->isEnhanced() )
		{
			event.posX -= mouseContain->getX();
			event.posY -= mouseContain->getY();
		}
		else
		{
			event.posX -= that->padLeft + mouseContain->getX();
			event.posY -= that->padTop + mouseContain->getY();
		}
		
		// Trigger the Event
		if( mouseContain->isDraggable() )
		{
			mouseContain->dragged = true;
			mouseContain->handleEvent( event );
		}
		else
			mouseContain->handleEventDefault( event );
		
		// Set Gadget, which receives all other drag Events until dragStop is called
		dragTemp = mouseContain;
		
		return handled;		
		
	}
	if( !dragTemp )
		return not_handled;// 'dragging'- or 'dragStop'-events without an object doesn't make sence
	
	// Make touch positions relative to parent
	if( dragTemp->isEnhanced() )
	{
		event.posX -= that->getX();
		event.posY -= that->getY();
	}
	else
	{
		event.posX -= that->padLeft + that->getX();
		event.posY -= that->padTop + that->getY();
	}
	
	// 'dragStop'
	if( event.getType() == dragStop )
	{
		// Rewrite Destination
		event.setDestination( dragTemp );
		
		if( dragTemp->dragged )
		{
			// Rewrite Destination and trigger the Event
			dragTemp->handleEvent( event );
			
			// Update _style
			dragTemp->dragged = false;
		}
		else
			dragTemp->handleEventDefault( event );
		
		if( dragTemp->pressed )
		{
			dragTemp->pressed = false;
			dragTemp->triggerEvent( onMouseLeave );
		}
		
		// No Gadget will receive Events anymore
		dragTemp = nullptr;
		
		return handled;
	}
	
	//!
	//! Control will only get here on 'dragging'-events
	//!
	if( // Check if we have to fire an 'onMouseLeave'-event
		!dragTemp->isDraggable() // Do not fire if 'dragTemp' is draggable, because it would look strange on some gadgets (e.g. scrollbutton)
		&& dragTemp->pressed
		&& !dragTemp->dimensions.contains( event.getPosX() , event.getPosY() )
	){
		dragTemp->pressed = false;
		dragTemp->triggerEvent( onMouseLeave );
	}
	
	// Rewrite Destination
	event.setDestination( dragTemp );
	
	if( dragTemp->dragged )
	{			
		// Rewrite Destination andTrigger the Event
		dragTemp->handleEvent( event );
	}
	else
		dragTemp->handleEventDefault( event );
	
	return handled;
}

// Convert a gadgetType to a string
map<_gadgetType,string> gadgetType2string = {
	{ _gadgetType::button , "button" },
	{ _gadgetType::label , "label" },
	{ _gadgetType::checkbox , "checkbox" },
	{ _gadgetType::radiobox , "radiobox" },
	{ _gadgetType::textbox , "textbox" },
	{ _gadgetType::textarea , "textarea" },
	{ _gadgetType::selectbox , "selectbox" },
	{ _gadgetType::selectitem , "selectitem" },
	{ _gadgetType::progressbar , "progressbar" },
	{ _gadgetType::keyboard , "keyboard" },
	{ _gadgetType::desktop , "desktop" },
	{ _gadgetType::fileview , "fileview" },
	{ _gadgetType::fileobject , "fileobject" },
	{ _gadgetType::imagegadget , "imagegadget" },
	{ _gadgetType::scrollarea , "scrollarea" },
	{ _gadgetType::scrollbutton , "scrollbutton" },
	{ _gadgetType::scrollbar , "scrollbar" },
	{ _gadgetType::window , "window" },
	{ _gadgetType::screen , "screen" },
	{ _gadgetType::contextmenu , "contextmenu" },
	{ _gadgetType::_plain , "_plain" }
};