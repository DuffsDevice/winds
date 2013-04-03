#include "_type/type.gadget.h"
#include "_type/type.gadgetScreen.h"
#include "_type/type.system.h"

_array<_callback*,13>
	_gadget::defaultEventHandlers = {
	/* refresh */ new _staticCallback( &_gadget::gadgetRefreshHandler ) ,
	/* mouseClick */ new _staticCallback( &_gadget::gadgetMouseHandler ) ,
	/* mouseDoubleClick */ new _staticCallback( &_gadget::gadgetMouseHandler ) ,
	/* mouseDown */ new _staticCallback( &_gadget::gadgetMouseHandler ) ,
	/* mouseUp */ new _staticCallback( &_gadget::gadgetMouseHandler ) ,
	/* mouseRepeat */ new _staticCallback( &_gadget::gadgetMouseHandler ) ,
	/* keyDown */ new _staticCallback( &_gadget::gadgetKeyHandler ) ,
	/* keyUp */ new _staticCallback( &_gadget::gadgetKeyHandler ) ,
	/* keyClick */ new _staticCallback( &_gadget::gadgetKeyHandler ) ,
	/* keyRepeat */ new _staticCallback( &_gadget::gadgetKeyHandler ) ,
	/* dragStart */ new _staticCallback( &_gadget::gadgetDragHandler ) ,
	/* dragStop */ new _staticCallback( &_gadget::gadgetDragHandler ) ,
	/* dragging */ new _staticCallback( &_gadget::gadgetDragHandler )
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


void _gadget::triggerEvent( _event&& event )
{
	_system::generateEvent( event.setDestination( this ) );
}


void _gadget::populateEvent( _event&& event )
{
	_system::generateEvent( event );
}


void _gadget::bubbleRefresh( bool includeThis , _event&& event )
{
	// Bubble!
	if( this->parent )
	{
		if( includeThis )
			this->handleEventDefault( event );
		
		_rect dim = this->parent->getAbsoluteDimensions();
		
		event.getDamagedRects().clipToIntersect( dim );
		
		if( event.hasClippingRects() )
			this->parent->bubbleRefresh( true , (_event&&)event ); // Forces std::move
	}
	else if( includeThis )
		this->handleEventDefault( (_event&&)event );
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
	if( this->counter > 5 )
	{
		_system::terminateTimer( _classCallback( this , &_gadget::blinkHandler ) );
		this->counter = 0;
		return;
	}
	if( this->counter++ % 2 )
		this->show();
	else
		this->hide();
}


void _gadget::blink()
{
	_system::terminateTimer( _classCallback( this , &_gadget::blinkHandler ) );
	_system::executeTimer( new _classCallback( this , &_gadget::blinkHandler ) , 70 , true );
}


bool _gadget::hasClickRights() const {
	if( this->hasFocus() || !this->style.canTakeFocus || !this->style.canReceiveFocus )
		return true;
	
	_gadget* cur = this->parent;
	while( cur )
	{
		if( !cur->style.canReceiveFocus || !this->style.canTakeFocus )
			return true;
		cur = cur->parent;
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
		child->bubbleRefresh();
	}
	
	return true;
}


void _gadget::unregisterEventHandler( _eventType type )
{
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
	_s32 posInArray = _s32( event.getType() ) - 2;
	
	// Use the default EventHandler if available
	if( posInArray >= 0 && _u32(posInArray) < defaultEventHandlers.size() )
	{
		event.setGadget( this );
		return (*defaultEventHandlers[ posInArray ])( (_event&&)event );
	}
	
	// If the Handler for the given event doesn't exist, return 
	return not_handled;
}


_callbackReturn _gadget::handleEvent( _event&& event )
{
	_eventHandlerMap::const_iterator it = this->eventHandlers.find( event.getType() );
	
	// Check for Normal Event Handlers
	if( it != this->eventHandlers.end() )
	{
		event.setGadget( this );
		_callbackReturn ret = (*it->second)( event );
		if( ret != use_default )
			return ret;
	}
	
	// If the Handler doesn't exist, return the default Handler
	return this->handleEventDefault( (_event&&)event );
}

_callbackReturn _gadget::handleEventUser( _event&& event )
{
	_eventHandlerMap::const_iterator it = this->eventHandlers.find( event.getType() );
	
	// Check for Normal Event Handlers
	if( it != this->eventHandlers.end() )
	{
		event.setGadget( this );
		return (*it->second)( (_event&&)event );
	}
	return not_handled;
}


void _gadget::setX( _coord val )
{
	int diff = val - this->x;
	
	if( diff )
	{
		_2s32 pos = this->getAbsolutePosition();
		this->x = val;
		this->bubbleRefresh( false , _event::refreshEvent( _rect( pos.first , pos.second , this->width , this->height ).combine( _rect( pos.first + diff , pos.second , this->width , this->height ) ) ) );
	}
}


void _gadget::setY( _coord val )
{
	int diff = val - this->y;
	
	if( diff )
	{
		_2s32 pos = this->getAbsolutePosition();
		this->y = val;
		this->bubbleRefresh( false ,
			_event::refreshEvent( 
				_rect( pos.first , pos.second , this->width , this->height )
				.combine( _rect( pos.first , pos.second + diff , this->width , this->height ) )
			)
		);
	}
}


void _gadget::moveRelative( _s16 dX , _s16 dY )
{
	if( !dX && !dY )
		return;
	
	_2s32 pos = this->getAbsolutePosition();
	
	this->x += dX;
	this->y += dY;
	
	this->bubbleRefresh( false ,
			_event::refreshEvent( 
				_rect( pos.first , pos.second , this->width , this->height )
				.combine( _rect( pos.first + dX , pos.second + dY , this->width , this->height ) )
			)
		);
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
	this->bubbleRefresh( true );
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
	
	// Erase the Connection on both sides
	if( child->isEnhanced() )
		this->enhancedChildren.remove( child );
	else
		this->children.remove( child );
	
	child->parent = nullptr;
	
	// Make it appear deleted
	this->bubbleRefresh( true , _event::refreshEvent( { absDim } ) );
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
	child->dragged = false;
	
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
	child->dragged = false;
	
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
		this->bubbleRefresh( true , _event::refreshEvent( absDim.combine( newAbsDim ) ) );
	}
	else if( newAbsDim.x != absDim.x || newAbsDim.y != absDim.y )
		// Delete the parts that originally were gadget, but became damaged
		this->bubbleRefresh( false , _event::refreshEvent( absDim.combine( newAbsDim ) ) );
}


void _gadget::setHeight( _length val )
{
	if( int(val) < 1 )
		val = 1;
	
	if( !this->isResizeableY() || val == this->height )
		return;
	
	_rect dim = this->getAbsoluteDimensions();
	dim.height = max( val , dim.height );
	
	this->height = val;
	this->bitmap.setHeight( val );
	
	this->handleEvent( onResize );
	
	// Delete the parts that originally were gadget, but became damaged
	this->bubbleRefresh( true , _event::refreshEvent( dim ) );
}


void _gadget::setWidth( _length val )
{	
	if( !this->isResizeableX() || val == this->width )
		return;
	
	if( int(val) < 1 )
		val = 1;
	
	_rect dim = this->getAbsoluteDimensions();
	dim.width = max( val , dim.width );
	
	this->width = val;
	this->bitmap.setWidth( val );
	
	this->handleEvent( onResize );
	
	// Delete the parts that originally were gadget, but became damaged
	this->bubbleRefresh( true , _event::refreshEvent( dim ) );
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


_callbackReturn _gadget::gadgetRefreshHandler( _event&& event )
{
	_gadget* that = event.getGadget();
	
	// Break up, if there are no children to paint!
	if( that->children.empty() && that->enhancedChildren.empty() )
		goto refreshEnd;
	
	{ //! New Section
		
		// Receive Bitmap-Port
		_bitmapPort bP = that->getBitmapPort();
		
		//! Set the available Area for the gadget
		if( event.hasClippingRects() )
			bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
		else
			bP.addClippingRects( _rect( 0 , 0 , that->width , that->height ) );
		
		//! Draw All enhanced Children
		for( _gadget* gadget : that->enhancedChildren )
		{
			if( gadget->isInvisible() )
				continue;
			
			_rect dim = gadget->getDimensions();
			
			// Copy...
			bP.copyTransparent( dim.x , dim.y , gadget->getBitmap() );
			
			// Reduce Painting Area
			bP.clippingRects.reduce( dim );
		}
		
		_rect clip = that->getSize().applyPadding( that->getPadding() );
		
		
		// Store padding in termporaries
		_length padLeft = that->padLeft;
		_length padTop = that->padTop;
		
		
		//! Draw all other children
		for( _gadget* gadget : that->children )
		{
			if( gadget->isInvisible() )
				continue;
			
			_rect dim = gadget->getDimensions().toRelative( -padLeft , -padTop ).clipToIntersect( clip );
			
			// Copy...
			bP.copyTransparent( dim.x , dim.y , gadget->getBitmap() );
			
			// Reduce Painting Area
			bP.clippingRects.reduce( dim );
		}
		
		event.setDamagedRects( bP.clippingRects.toRelative( _2s32( -that->getAbsoluteX() , -that->getAbsoluteY() ) ) );
		
	} //! New Section end
	
	/* Label */
	refreshEnd:
	
	// Refresh 'my content'
	if( event.hasClippingRects() )
		that->handleEventUser( event );
	
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


_callbackReturn _gadget::gadgetMouseHandler( _event&& event )
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
		
		//! Do not try to focus the child of a gadget that can't even be clicked!
		if( event.getType() == mouseDown && mouseContain->isClickable() )
		{
			// Update _lastClickedGadget_ so that specialities like hasSmallDragTrig work! See _gadgetScreen::processTouch()
			_system::_lastClickedGadget_ = mouseContain;
			
			// Try to focus the child
			that->focusChild( mouseContain );
		}
		/** 
		 * @note The previous 'if'-block had to go first to allow a gadget to be focused.
		 *       This wouldn't be possible if it was written inside the 'if'-block underneath,
		 *       because 'hasClickRigths' checks if the gadget is focused
		 */
		
		// Trigger the Event if the gadget is now focused or if it never will have any focus,
		if( mouseContain->hasClickRights() )
		{
			// If a child is clicked, abort
			_callbackReturn ret = mouseContain->handleEventDefault( event );
			
			if( ret != not_handled )
				return ret;
			
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
			
			// Call the user-registered event-handler
			mouseContain->handleEventUser( (_event&&)event );
			
			return handled;
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


_callbackReturn _gadget::gadgetDragHandler( _event&& event )
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
		dragTemp = mouseContain;
		
		_callbackReturn ret = mouseContain->handleEventDefault( event );
		
		// Mark the gadget as 'dragged' (but only if no child wants to be dragged -> check that first)
		if( ret == not_handled )
		{
			if(
				mouseContain->isDraggable()
				&& mouseContain->isClickable()
				&& mouseContain->handleEventUser( event ) != prevent_default
			)
				mouseContain->dragged = true;
			else
				return not_handled;
		}
		
		return handled;
	}
	
	if( !dragTemp )
		return prevent_default; // 'dragging'- or 'dragStop'-events without an object doesn't make sence
	
	// Make touch positions relative to parent
	event.posX -= that->getX();
	event.posY -= that->getY();
	
	// if the child is not enhanced
	if( !dragTemp->isEnhanced() )
	{
		event.posX -= that->padLeft;
		event.posY -= that->padTop;
	}
	
	// Rewrite Destination
	event.setDestination( dragTemp );
	
	// 'dragStop'
	if( event.getType() == dragStop )
	{
		if( dragTemp->dragged )
		{
			// Rewrite Destination and trigger the Event
			dragTemp->handleEvent( event );
			
			// Update _style
			dragTemp->dragged = false;
		}
		else
			dragTemp->handleEventDefault( event ); // Forward the dragStop-event to the children
		
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
	
	if( dragTemp->dragged )
	{
		// Rewrite Destination andTrigger the Event
		if( dragTemp->handleEvent( event ) == prevent_default )
		{
			dragTemp->dragged = false; // If the dragged gadget no longer wants to be dragged
			return prevent_default; // Force the upper gadget also to stop dragging
		}
	}
	else
		return dragTemp->handleEventDefault( event );
	
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