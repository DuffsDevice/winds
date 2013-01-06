#include "_type/type.gadget.h"
#include "_type/type.system.h"

map<_eventType,_gadgetDefaultEventHandler> _gadget::defaultEventHandlers = {
	{ focus , &_gadget::gadgetFocusHandler },
	{ blur , &_gadget::gadgetFocusHandler },
	{ mouseDown , &_gadget::gadgetMouseHandler },
	{ mouseUp , &_gadget::gadgetMouseHandler },
	{ mouseClick , &_gadget::gadgetMouseHandler },
	{ mouseDoubleClick , &_gadget::gadgetMouseHandler },
	{ refresh , &_gadget::gadgetRefreshHandler },
	{ dragStart , &_gadget::gadgetDragHandler },
	{ dragStop , &_gadget::gadgetDragHandler },
	{ dragging , &_gadget::gadgetDragHandler },
	{ keyDown , &_gadget::gadgetKeyHandler },
	{ keyUp , &_gadget::gadgetKeyHandler },
	{ keyClick , &_gadget::gadgetKeyHandler },
};

_gadget::_gadget( _gadgetType type , int width , int height , int posX , int posY , _style style , bool doNotAllocateBitmap )
	: type( type ) , padding( _padding( 0 ) ) , dimensions( _rect( posX , posY , max( 1 , width ) , max( 1 , height ) ) ) , style( style ) , focusedChild( nullptr ) , parent( nullptr )  , dragTemp ( nullptr )
{
	if( !doNotAllocateBitmap )
		this->bitmap = _bitmap( this->getWidth() , this->getHeight() );
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
	this->handleEvent( _event( this , refresh ).preventBubble( true ) );
}

void _gadget::generateEvent( _event event ){
	_system::generateEvent( event );
}

void _gadget::bubbleRefresh( bool includeThis , _event event )
{
	// Event not generated yet
	if( !event.hasClippingRects() )
		event = _event::refreshEvent( this , { this->getAbsoluteDimensions() } );
	
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

_gadget* _gadget::getScreen()
{
	if( this->type == _gadgetType::screen )
		return this;
	else if( this->parent != nullptr )
		return this->parent->getScreen();
	else
		return nullptr;
}

int _gadget::unregisterLuaEventHandler( _eventType type ){
	
	if( this->luaEventHandlers.count( type ) == 0 )
		return LUA_NOREF;
	
	int d = this->luaEventHandlers[ type ].first;
	
	// Unbind the Handler
	this->luaEventHandlers.erase( type );
	
	return d;
}

bool _gadget::blurChild()
{
	if( focusedChild )
	{
		if( !focusedChild->style.canLooseFocus || !focusedChild->blurChild() )
			return false;
		focusedChild->style.focused = false;
		_gadget* t = focusedChild;
		focusedChild = nullptr;
		t->handleEvent( _event( onBlur ) );
		_system::_currentFocus_ = nullptr;
	}
	return true;
}

bool _gadget::focusChild( _gadget* child )
{
	if( !child || !child->style.canTakeFocus )
		return false;
	if( child->style.focused )
		return true;
	
	// Blur the Previously style.focused gadget
	if( !this->blurChild() )
		return false;
	
	if( child->style.canReceiveFocus )
	{
		_system::_currentFocus_ = child;
		child->style.focused = true;
		focusedChild = child;
		focusedChild->handleEvent( _event( onFocus ) );
		
		// Move to top
		if( child->isEnhanced() )
		{
			if( child == this->enhancedChildren.front() )
				return true;
			this->enhancedChildren.remove( child );
			this->enhancedChildren.push_front( child );
		}
		else
		{
			if( child == this->children.front() )
				return true;
			this->children.remove( child );
			this->children.push_front( child );
		}
		
		// Refresh the Gadget
		child->bubbleRefresh();
	}
	
	return true;
}

_callbackReturn _gadget::handleEventDefault( _event& event )
{
	// Use the default EventHandler if available
	if( _gadget::defaultEventHandlers.count( event.getType() ) )
		return _gadget::defaultEventHandlers[ event.getType() ]( event );
	
	// If the Handler for the given event doesn't exist, return 
	return not_handled;
}

_callbackReturn _gadget::handleEventNormal( _event event )
{
	// Check for Normal Event Handlers
	if( this->eventHandlers.count( event.getType() ) )
	{
		_callbackReturn ret = this->eventHandlers[ event.getType() ]( event );
		
		if( ret != use_default )
			return ret;
	}
	
	// If the Handler doesn't exist, return 
	return this->handleEventDefault( event );
}

_callbackReturn _gadget::handleEvent( _event event )
{
	//startTimer( reinterpret_cast<void*>(&_gadget::handleEvent) );
	event.setGadget( this );
	pair<int,lua_State*> data;
	_callbackReturn ret = not_handled;
	
	// Check for a Handler by lua
	if( this->luaEventHandlers.count( event.getType() ) )
	{
		ret = lua_callEventHandler( (data = this->luaEventHandlers[ event.getType() ]).second , data.first , event );
		if( ret == use_default )
		{
			return this->handleEventDefault( event ); // Use the default EventHandler if available	
		}
		else if( ret != use_normal )
		{
			return ret;
		}
	}
	// Use the default EventHandler if available
	return this->handleEventNormal( event ); 
}

_coord _gadget::getAbsoluteX() const {
	if( this->parent != nullptr )
		return this->parent->getAbsoluteX() + this->dimensions.x + ( this->style.enhanced ? 0 : this->parent->getPadding().getLeft() );
	
	return this->dimensions.x;
}

_coord _gadget::getAbsoluteY() const {
	if( this->parent != nullptr )
		return this->parent->getAbsoluteY() + this->dimensions.y + ( this->style.enhanced ? 0 : this->parent->getPadding().getTop() );
	
	return this->dimensions.y;
}

void _gadget::setX( _coord val ){
	if( val == this->dimensions.x )
		return;
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.x = val;
	this->bubbleRefresh( false , _event::refreshEvent( this , dim.combine( this->getAbsoluteDimensions() ) ) );
}

void _gadget::setY( _coord val ){
	if( val == this->dimensions.y )
		return;
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.y = val;
	this->bubbleRefresh( false , _event::refreshEvent( this , dim.combine( this->getAbsoluteDimensions() ) ) );
}

void _gadget::moveRelative( _s16 dX , _s16 dY ){
	if( !dX && !dY )
		return;
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.x += dX;
	this->dimensions.y += dY;
	this->bubbleRefresh( false , _event::refreshEvent( this , dim.combine( this->getAbsoluteDimensions() ) ) );
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
	this->bubbleRefresh( false , _event::refreshEvent( this , dim.combine( this->getAbsoluteDimensions() ) ) );
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
	
	if( child->isEnhanced() )
		this->enhancedChildren.remove( child );
	else
		this->children.remove( child );
	
	this->bubbleRefresh( true , _event::refreshEvent( this , { absDim } ) );
	
	if( child->style.focused )
		this->blurChild();
	
	// Erase the Connection on both sides
	child->parent = nullptr;
}

void _gadget::addChild( _gadget* child )
{	
	if( !child )
		return;
	
	// Add it!
	this->children.push_front( child );
	
	child->style.focused = false;
	child->style.enhanced = false;
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
	
	child->style.focused = false;
	child->style.enhanced = true;
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
	_rect dim = this->getDimensions();
	_rect absDim = this->getAbsoluteDimensions();
	this->dimensions = rc;
	
	if( dim.width != this->dimensions.width || this->dimensions.height != dim.height )
	{
		this->bitmap.resize( rc.width , rc.height );
		this->handleEvent( _event( onResize ) );
		this->bubbleEvent( _event::refreshEvent( this , absDim.combine( this->getAbsoluteDimensions() ) ) , true );
	}
	else if( dim.x != this->dimensions.x || dim.y != this->dimensions.y )
		// Delete the parts that originally were gadget, but became damaged
		this->bubbleEvent( _event::refreshEvent( this , absDim.combine( this->getAbsoluteDimensions() ) ) );
}

void _gadget::setHeight( _length val )
{
	if( val == this->dimensions.height )
		return;
	
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.height = val;
	this->bitmap.setHeight( val );
	
	this->handleEvent( _event( onResize ) );
	
	// Delete the parts that originally were gadget, but became damaged
	this->bubbleEvent( _event::refreshEvent( this , dim.combine( this->getAbsoluteDimensions() ) ) , true );
}

void _gadget::setWidth( _length val )
{
	if( val == this->dimensions.width )
		return;
	
	_rect dim = this->getAbsoluteDimensions();
	this->dimensions.width = val;
	this->bitmap.setWidth( val );
	
	this->handleEvent( _event( onResize ) );
	
	// Delete the parts that originally were gadget, but became damaged
	this->bubbleEvent( _event::refreshEvent( this , dim.combine( this->getAbsoluteDimensions() ) ) , true );
}

_callbackReturn _gadget::gadgetRefreshHandler( _event& event )
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
		_rect dim = gadget->getDimensions().relativeVersion( -padding.getLeft() , -padding.getTop() );
		
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
	_coord posPadX = posX - p.getLeft();
	_coord posPadY = posY - p.getTop();
	
	for( _gadget* gadget : parent->enhancedChildren )
	{
		// Check if event position was inside this Gadget's Area
		if( gadget->getDimensions().contains( posX , posY ) )
			return gadget;
	}
	
	for( _gadget* gadget : parent->children )
	{
		// Check if event position was inside this Gadget's Area
		if( gadget->getDimensions().contains( posPadX , posPadY ) )
			return gadget;
	}
	
	return nullptr;
}

_callbackReturn _gadget::gadgetMouseHandler( _event& event )
{
	_gadget* that = event.getGadget();
	
	const _padding& p = that->getPadding();
	
	_coord posX = event.getPosX();
	_coord posY = event.getPosY();
	_coord posPadX = posX - p.getLeft();
	_coord posPadY = posY - p.getTop();
	
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
		
		// It doesn't make sense to focus a child of some _gadget that can't be style.focused
		if( event.getType() == mouseDown )
			that->focusChild( mouseContain );
		else if( event.getType() == mouseDoubleClick && !mouseContain->canReactTo( mouseDoubleClick ) )
			event.setType( mouseClick );
		
		// Trigger the Event
		return mouseContain->handleEvent( event );
	}
	
	// If no Gadget received the Mousedown, blur the Focussed Child
	if( event.getType() == mouseDown )
		that->blurChild();
	
	return not_handled;
}


_callbackReturn _gadget::gadgetDragHandler( _event& event )
{
	// Temp...
	_gadget* that = event.getGadget();
	
	// Temp...
	const _padding& p = that->getPadding();
	_coord posX = event.getPosX();
	_coord posY = event.getPosY();
	_coord posPadX = posX - p.getLeft();
	_coord posPadY = posY - p.getTop();
	
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
		if( that->dragTemp->style.enhanced )
			event	.setPosX( posX - that->getX() )
					.setPosY( posY - that->getY() );
		else
			event	.setPosX( posX - ( that->getX() + p.getLeft() ) )
					.setPosY( posY - ( that->getY() + p.getTop() ) );
		
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
		if( that->dragTemp->style.enhanced )
			event	.setPosX( posX - that->getX() )
					.setPosY( posY - that->getY() );
		else
			event	.setPosX( posX - ( that->getX() + p.getLeft() ) )
					.setPosY( posY - ( that->getY() + p.getTop() ) );
		
		// Rewrite Destination andTrigger the Event
		return that->dragTemp->handleEvent( event );
	}
	return not_handled;
}

_callbackReturn _gadget::gadgetFocusHandler( _event& event )
{
	_gadget* that = event.getGadget();
	
	if( !that->parent )
		return not_handled;
	
	if( event.getType() == focus )
		that->parent->focusChild( that );
	else if( event.getType() == blur )
		that->parent->blurChild();
	
	return handled;
}

_callbackReturn _gadget::gadgetKeyHandler( _event& event )
{
	_gadget* that = event.getGadget();
	
	if( that->parent )
		that->parent->handleEvent( event );
	
	return not_handled;
}