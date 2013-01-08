#include "_type/type.gadget.h"
#include "_type/type.gadgetScreen.h"
#include "_type/type.system.h"

_map<_eventType,const _callback*> _gadget::defaultEventHandlers = {
	{ focus , new _staticCallback( &_gadget::gadgetFocusHandler ) },
	{ blur , new _staticCallback( &_gadget::gadgetFocusHandler ) },
	{ mouseDown , new _staticCallback( &_gadget::gadgetMouseHandler ) },
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
};

_gadget::_gadget( _gadgetType type , int width , int height , int posX , int posY , _style style , bool doNotAllocateBitmap )
	: type( type ) , padding( _padding( 0 ) ) , dimensions( _rect( posX , posY , max( 1 , width ) , max( 1 , height ) ) ) , style( style ) , focusedChild( nullptr ) , parent( nullptr )  , dragTemp ( nullptr )
{
	if( !doNotAllocateBitmap )
		this->bitmap = _bitmap( this->getWidth() , this->getHeight() );
	
	this->registerEventHandler( mouseDoubleClick , new _staticCallback( &_gadget::gadgetMouseHandler ) );
	
	this->triggerEvent( onStyleSet );
}

_gadgetType typeOfGadget( _gadget* g )
{
	return g->getType();
}

//
// Delegating Constructors!!!! C++0x I love you!
//
_gadget::_gadget( int width , int height , int posX , int posY , _style style , bool doNotAllocateBitmap )
	: _gadget( _gadgetType::_plain , width , height , posX , posY , style , doNotAllocateBitmap )
{ }

_gadget::~_gadget()
{
	if( this->parent != nullptr )
		this->parent->removeChild( this );
	
	// Unbind event Handler
	for( const _pair<_eventType,const _callback*>& data : this->eventHandlers )
	{
		if( data.second )
			delete data.second;
	}
}

void _gadget::setPadding( const _padding& p )
{
	if( this->padding != p )
	{
		this->padding = p;
		this->bubbleRefresh( true );
	}
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

void _gadget::refreshBitmap()
{
	this->handleEvent( _event( refresh ).preventBubble( true ) );
}

void _gadget::generateEvent( _event event ){
	_system::generateEvent( event );
}

void _gadget::bubbleRefresh( bool includeThis , _event event )
{
	// Event not generated yet
	if( !event.hasClippingRects() )
		event = _event::refreshEvent( { this->getAbsoluteDimensions() } );
	
	if( includeThis )
		this->handleEvent( event );
	
	// Bubble!
	if( this->parent != nullptr )
	{
		_rect size = this->parent->getAbsoluteDimensions();
		
		if( size.intersectsWith( event.getDamagedRects() ) )
			this->parent->bubbleRefresh( true , event );
	}
}

_gadgetScreen* _gadget::getScreen()
{
	if( this->type == _gadgetType::screen )
		return (_gadgetScreen*)this;
	else if( this->parent != nullptr )
		return this->parent->getScreen();
	return nullptr;
}

void _gadget::blinkHandler()
{
	if( this->style.unused2 > 5 )
	{
		_system::terminateTimer( _classCallback( this , &_gadget::blinkHandler ) );
		this->style.unused2 = 0;
		return;
	}
	if( this->style.unused2++ % 2 )
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
		
		this->focusedChild->style.focused = false;
		
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
	if( !child || !child->isVisible() || child->isMinimized() )
		return false;
	
	// Return true if the child already has focus
	if( child->hasFocus() )
		return true;
	
	// Blur the Previously style.focused gadget (try to)
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
	
	// Set _currentFocus_
	_system::_currentFocus_ = child;
	
	// 'focus' the child
	child->style.focused = true;
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

_callbackReturn _gadget::handleEventDefault( _event event )
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

_callbackReturn _gadget::handleEvent( _event event )
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

_coord _gadget::getAbsoluteX() const {
	if( this->parent != nullptr )
		return this->parent->getAbsoluteX() + this->dimensions.x + ( this->isEnhanced() ? 0 : this->parent->getPadding().left );
	
	return this->dimensions.x;
}

_coord _gadget::getAbsoluteY() const {
	if( this->parent != nullptr )
		return this->parent->getAbsoluteY() + this->dimensions.y + ( this->isEnhanced() ? 0 : this->parent->getPadding().top );
	
	return this->dimensions.y;
}

void _gadget::setX( _coord val ){
	if( val == this->dimensions.x )
		return;
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.x = val;
	this->bubbleRefresh( false , _event::refreshEvent( dim.combine( this->getAbsoluteDimensions() ) ) );
}

void _gadget::setY( _coord val ){
	if( val == this->dimensions.y )
		return;
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.y = val;
	this->bubbleRefresh( false , _event::refreshEvent( dim.combine( this->getAbsoluteDimensions() ) ) );
}

void _gadget::moveRelative( _s16 dX , _s16 dY ){
	if( !dX && !dY )
		return;
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.x += dX;
	this->dimensions.y += dY;
	this->bubbleRefresh( false , _event::refreshEvent( dim.combine( this->getAbsoluteDimensions() ) ) );
}

void _gadget::moveTo( _coord x , _coord y )
{
	if( x == this->dimensions.x && y == this->dimensions.y )
		return;
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.x = x;
	this->dimensions.y = y;
	//if( this->parent )
	//	this->parent->bitmap->move( dim.x , dim.y , this->dimensions.x , this->dimensions.y , this->dimensions.width , this->dimensions.height );
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
		this->children.remove_if( this->removeCallback );
	else
		this->children.remove_if( this->removeDeleteCallback );
	
	//! Signalize there are no children left!
	this->bubbleRefresh( true );
}

void _gadget::removeEnhancedChildren( bool remove )
{
	if( this->enhancedChildren.empty() )
		return;
	
	if( remove )
		this->enhancedChildren.remove_if( this->removeCallback );
	else
		this->enhancedChildren.remove_if( this->removeDeleteCallback );
	
	//! Signalize there are no children left!
	this->bubbleRefresh( true );
}

void _gadget::removeChild( _gadget* child )
{
	if( !child || this->children.empty() )
		return;
		
	// Erase it on my bitmap
	_rect absDim = child->getAbsoluteDimensions();
	
	// Remove focus
	if( child->hasFocus() )
	{
		child->style.focused = false;
		
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
	child->style.focused = false;
	child->style.enhanced = false;
	child->style.visible = true;
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
	child->style.focused = false;
	child->style.enhanced = true;
	child->style.visible = true;
	child->parent = this;
	
	//! Paint it on my bmp
	child->bubbleRefresh( true );
}

_rect _gadget::getAbsoluteDimensions() const { 
	_rect out = this->dimensions;
	out.x = this->getAbsoluteX();
	out.y = this->getAbsoluteY();
	return out;
}

void _gadget::setDimensions( _rect rc )
{
	//if( !rc.isValid() )
	//	return;
	
	_rect absDim = this->getAbsoluteDimensions();
	
	// Respect Fixed width/height of the gadget
	if( !this->isResizeableX() )
		rc.width = this->dimensions.width;
	if( !this->isResizeableY() )
		rc.height = this->dimensions.height;
	
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
	if( !val )
		val = 1;
	
	if( !this->isResizeableY() || val == this->dimensions.height )
		return;
	
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.height = val;
	this->bitmap.setHeight( val );
	
	this->handleEvent( onResize );
	
	// Delete the parts that originally were gadget, but became damaged
	this->bubbleEvent( _event::refreshEvent( dim.combine( this->getAbsoluteDimensions() ) ) , true );
}

void _gadget::setWidth( _length val )
{
	if( !val )
		val = 1;
	
	if( !this->isResizeableX() || val == this->dimensions.width )
		return;
	
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.width = val;
	this->bitmap.setWidth( val );
	
	this->handleEvent( onResize );
	
	// Delete the parts that originally were gadget, but became damaged
	this->bubbleEvent( _event::refreshEvent( dim.combine( this->getAbsoluteDimensions() ) ) , true );
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
	_rect areaAvailable = { 0 , 0 , that->dimensions.width , that->dimensions.height };
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
		damagedEnhancedRects.add( _rect( 0 , 0 , that->dimensions.width , that->dimensions.height ) );
	}
	
	const _padding& padding = that->getPadding();
	
	for( _gadget* gadget : that->children )
	{
		if( !gadget->isVisible() || gadget->isMinimized() )
			continue;
		
		_rect dim = gadget->getDimensions().relativeVersion( -padding.left , -padding.top );
		
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
		if( !gadget->isVisible() || gadget->isMinimized() )
			continue;
		const _rect& dim = gadget->getDimensions();
		
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


_gadget* _gadget::getGadgetOfMouseDown( _coord posX , _coord posY , _gadget* parent)
{
	const _padding& p = parent->getPadding();
	_coord posPadX = posX - p.left;
	_coord posPadY = posY - p.top;
	
	for( _gadget* gadget : parent->enhancedChildren )
	{
		// Check if event position was inside this Gadget's Area
		if( gadget->isVisible() && !gadget->isMinimized() && gadget->getDimensions().contains( posX , posY ) )
			return gadget;
	}
	
	for( _gadget* gadget : parent->children )
	{
		// Check if event position was inside this Gadget's Area
		if( gadget->isVisible() && !gadget->isMinimized() && gadget->getDimensions().contains( posPadX , posPadY ) )
			return gadget;
	}
	
	return nullptr;
}

_callbackReturn _gadget::gadgetMouseHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	const _padding& p = that->getPadding();
	
	_coord posX = event.getPosX();
	_coord posY = event.getPosY();
	_coord posPadX = posX - p.left;
	_coord posPadY = posY - p.top;
	
	// Temp...
	_gadget* mouseContain = getGadgetOfMouseDown( posX , posY , that );
	
	if( mouseContain )
	{
		// Rewrite Destination
		event.setDestination( mouseContain );
		
		// Absolute Position to Relative Position
		if( mouseContain->isEnhanced() )
			event	.setPosX( posX - mouseContain->getX() )
					.setPosY( posY - mouseContain->getY() );
		else
			event	.setPosX( posPadX - mouseContain->getX() )
					.setPosY( posPadY - mouseContain->getY() );
		
		// It doesn't make sense to focus a child of some _gadget that can't be focused
		if( event.getType() == mouseDown )
			that->focusChild( mouseContain );
		else if( event.getType() == mouseDoubleClick && !mouseContain->canReactTo( mouseDoubleClick ) )
			event.setType( mouseClick );
		
		// Trigger the Event if the gadget is now focused or if it never will have any focus,
		// because then it wouldn't make any sence to forward the event!
		if( mouseContain->hasFocus() || !mouseContain->style.canTakeFocus || !mouseContain->style.canReceiveFocus )
			return mouseContain->handleEvent( event );
		return not_handled;
	}
	
	// If no Gadget received the Mousedown, blur the Focussed Child
	if( event.getType() == mouseDown )
		that->blurChild();
	
	return not_handled;
}


_callbackReturn _gadget::gadgetDragHandler( _event event )
{
	// Temp...
	_gadget* that = event.getGadget();
	
	// Temp...
	const _padding& p = that->getPadding();
	_coord posX = event.getPosX();
	_coord posY = event.getPosY();
	_coord posPadX = posX - p.left;
	_coord posPadY = posY - p.top;
	
	// Start Dragging
	if( event.getType() == dragStart )
	{
		// Temp...
		_gadget* mouseContain = getGadgetOfMouseDown( posX , posY , that );
		
		if( mouseContain )
		{
			// Rewrite Destination
			event.setDestination( mouseContain );
			
			// Absolute Position to Relative Position
			if( mouseContain->isEnhanced() )
				event	.setPosX( posX - mouseContain->getX() )
						.setPosY( posY - mouseContain->getY() );
			else
				event	.setPosX( posPadX - mouseContain->getX() )
						.setPosY( posPadY - mouseContain->getY() );
			
			// Trigger the Event if the gadget is now focused or if it never will have any focus,
			// because then it wouldn't make any sence to forward the event!
			if( !mouseContain->hasFocus() && mouseContain->style.canTakeFocus && mouseContain->style.canReceiveFocus )
				return not_handled;
			
			// Trigger the Event
			_callbackReturn ret = mouseContain->handleEvent( event );
			
			if( ret != not_handled )
			{
				// Set Gadget, which receives all other drag Events until dragStop is called
				that->dragTemp = mouseContain;
				mouseContain->style.dragged = true;
			}
			
			return ret;
		}
	}
	// Stop Dragging
	else if( that->dragTemp && event.getType() == dragStop )
	{
		// Absolute Position to Relative Position
		if( that->dragTemp->isEnhanced() )
			event	.setPosX( posX - that->getX() )
					.setPosY( posY - that->getY() );
		else
			event	.setPosX( posX - ( that->getX() + p.left ) )
					.setPosY( posY - ( that->getY() + p.top) );
		
		// Rewrite Destination andTrigger the Event
		_callbackReturn ret = that->dragTemp->handleEvent( event.setDestination( that->dragTemp ) );
		
		// Update _style
		that->dragTemp->style.dragged = false;
		
		// No Gadget will receive Events anymore
		that->dragTemp = nullptr;
		
		return ret;
	}
	else if( that->dragTemp && event.getType() == dragging )
	{
		// Absolute Position to Relative Position
		event.setDestination( that->dragTemp );
		
		// Absolute Position to Relative Position
		if( that->dragTemp->isEnhanced() )
			event	.setPosX( posX - that->getX() )
					.setPosY( posY - that->getY() );
		else
			event	.setPosX( posX - ( that->getX() + p.left ) )
					.setPosY( posY - ( that->getY() + p.top ) );
		
		// Rewrite Destination andTrigger the Event
		return that->dragTemp->handleEvent( event );
	}
	return not_handled;
}

_callbackReturn _gadget::gadgetFocusHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	if( !that->parent )
		return not_handled;
	
	if( event.getType() == focus )
		that->parent->focusChild( that );
	else if( event.getType() == blur && that->hasFocus() )
		that->parent->blurChild();
	
	return handled;
}

void _gadget::maximize()
{
	if( this->style.maximized || !this->isResizeable() )
		return;
	
	_gadgetScreen* screen = this->getScreen();
	
	// Fail
	if( screen == nullptr )
		return;
	
	// Fetch maximized Dimensions from the _screen
	_rect maxDim = screen->getMaximizedDimensions();
	
	if( maxDim.isValid() )
	{
		this->style.maximized = true;
		this->normalDimensions = this->dimensions;
		
		// Maximizing
		this->setDimensions( maxDim );
	}
}

void _gadget::unMaximize()
{
	if( !this->style.maximized )
		return;
	
	this->style.maximized = false;
	
	// Set back the old dimensions
	this->setDimensions( this->normalDimensions );
}

void _gadget::minimize()
{
	if( this->style.minimized || !this->style.minimizeable )
		return;
	
	// Blur
	this->handleEvent( blur );
	
	this->style.minimized = true;
	this->bubbleRefresh();
}

_callbackReturn _gadget::gadgetKeyHandler( _event event )
{
	_gadget* that = event.getGadget();
	
	if( that->parent )
		that->parent->handleEvent( event );
	
	return not_handled;
}