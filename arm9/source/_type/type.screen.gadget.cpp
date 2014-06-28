#include "_type/type.screen.gadget.h"
#include "_controller/controller.gui.h"

_gadgetScreen::_gadgetScreen( int bgId , _gadgetScreenType sType , _length width , _length height , _style&& style ) :
	_screen( bgId )
	, _gadget( _gadgetType::screen , 0 , 0 , width , height , _bitmap( _screen::getMemoryPtr() , width, height ) , (_style&&)style )
	, screenType( sType )
	, touchCycles( 0 )
	, framesSinceLastClick( 0 )
	, isDragging( false )
{}

_touch _gadgetScreen::adjustTouch( _touch touch )
{
	touch.x = int( float(touch.x) * this->getScaleX() ) + this->getScrollX();
	touch.y = int( float(touch.y) * this->getScaleY() ) + this->getScrollY();
	return touch;
}


bool _gadgetScreen::touchInside( _touch touch )
{
	return _rect( 0 , 0 , this->getWidth() , this->getHeight() ).contains( touch.x , touch.y ) // Inside my coordinates
		&& _color( this->getBitmap().getPixel( touch.x , touch.y ) ).getAlpha(); // Check if this part of the bitmap is non-transparent
}


bool _gadgetScreen::processTouch( bool held , _touch newTouch )
{
	// Temp...
	_event event = _event( _none_ ).setCurrentKeys( _hardwareKeyPattern::getCurrentKeys() );
	_gadget* lastClickedGadget = _guiController::getLastClickedGadget();
	
	// Increase clickCycles and prevent overflow
	if( this->framesSinceLastClick && !( this->framesSinceLastClick >> 15 ) )
		this->framesSinceLastClick++;
	
	// Adjust the pen position
	_touch  newNewTouch = _gadgetScreen::adjustTouch( newTouch );
	bool newTouchInside = this->touchInside( newNewTouch );
	
	/*!
	 * Handle the Pen!
	  */
	if( held )
	{
		event.setEffectivePosX( newTouch.x ).setEffectivePosY( newTouch.y ).setPosX( newNewTouch.x ).setPosY( newNewTouch.y );
		event.setPressure( 400 );
		
		//! Check if this is the first Frame the pen is down
		if( this->touchCycles == 0 )
		{
			// Trigger the Event
			if( newTouchInside )
				this->triggerEvent( event.setType( onMouseDown ) );
			
			// Set the starting Point of the mouseDown here!
			this->startTouch = newTouch;
			this->lastTouch = newTouch;
		}
		
		_touch newStartTouch = _gadgetScreen::adjustTouch( startTouch );
		
		// Check if the Pen is dragging something
		if( this->isDragging )
		{
			_touch newLastTouch = _gadgetScreen::adjustTouch( lastTouch ); // Adjust the lastTouch
			
			// Modify event and trigger
			event.setDeltaX( newNewTouch.x - newLastTouch.x ).setDeltaY( newNewTouch.y - newLastTouch.y );
			this->triggerEvent( event.setType( onDragging ) );
			
			this->lastTouch = newTouch; // Prepare for the next frame!
		}
		// Apparently not dragging! -> Check if the Stylus has moved, but only if its start point was inside this screen
		else if( this->touchInside( newStartTouch ) )
		{
			// Compute the dragged Distance: Pythagoras!
			_u32 xd = newTouch.x - startTouch.x;
			_u32 yd = newTouch.y - startTouch.y;
			_u32 dragDistance = xd * xd + yd * yd; // Actually its the square of the distance
			_u16 minDragThld = _guiController::getMinDragDistance();
			_u16 keyRepetitionDelay = _guiController::getKeyRepetitionDelay();
			_u16 keyRepetitionPause = _guiController::getKeyRepetitionPause();
			
			// For gadgets with a small dragging Trigger e.g. scrollBars
			if( lastClickedGadget && lastClickedGadget->wantsSmallDragThld() )
				minDragThld /= 4;
			
			// Check if Pen has moved the required distance already
			if( dragDistance > _u16(minDragThld * minDragThld) )
			{
				// Modify Event
				event.setEffectivePosX( startTouch.x ).setEffectivePosY( startTouch.y ).setPosX( newStartTouch.x ).setPosY( newStartTouch.y );
				
				// Start dragging
				this->triggerEvent( event.setType( onDragStart ) );
				this->isDragging = true;
				
				// That the delta distance between the start of the onDragStart and the first onDragging event
				// matches the distance the mouse had to move until dragging begun
				this->lastTouch = this->startTouch;
			}
			// If not dragging and If the current clicked gadget wants to have mouseClicks Repeating
			else if( lastClickedGadget ) // Do we have a gadget?
			{
				if( lastClickedGadget->wantsClickRepeat() )
				{
					if( keyRepetitionDelay > 0 && this->touchCycles > keyRepetitionDelay && this->touchCycles % keyRepetitionPause == 0 )
						this->triggerEvent( event.setType( onMouseRepeat ) );
				}
				else if( lastClickedGadget->isRightClickable() && this->touchCycles == _guiController::getRightClickDelay() )
				{
					this->triggerEvent( event.setType( onMouseRightClick ) );
				}
			}
		}
		
		this->touchCycles++; // Increase Cycles
		
	} // if( held ) {}
	
	// Pen is not down
	else if( this->touchCycles > 0 )
	{
		// Adjust the touch Position
		_touch newLastTouch = _gadgetScreen::adjustTouch( lastTouch );
		
		
		// Set parameters
		event.setEffectivePosX( lastTouch.x ).setEffectivePosY( lastTouch.y ).setPosX( newLastTouch.x ).setPosY( newLastTouch.y ).setPressure(0);		
		
		
		// Temporary variables
		_u16 maxDoubleClickPeriod	= _guiController::getMaxDoubleClickPeriod();
		_u16 maxClickPeriod			= _guiController::getMaxClickPeriod();
		_u16 maxDoubleClickAreaSqr	= pow( _guiController::getMaxDoubleClickArea() , 2 );
		bool lastTouchInside = this->touchInside( newLastTouch ); // Temporary flag that indicates whether the stylus was inside this screen last frame
		
		
		// Trigger mouseUpEvent if the mouseUp was performed inside this screen
		if( lastTouchInside )
			this->triggerEvent( event.setType( onMouseUp ) );
		
		
		// Maybe DragStop-Event?
		if( this->isDragging )
		{
			this->triggerEvent( event.setType( onDragStop ) );	// Trigger Event
			this->isDragging = false;							// Set touchDragging to false!
		}
		
		// Maybe Click-Event?
		else if( lastTouchInside && this->touchCycles < maxClickPeriod )
		{
			_s16	deltaX = touchBefore.x - lastTouch.x;
			_s16	deltaY = touchBefore.y - lastTouch.y;
			_length deltaTouchSqr = deltaX * deltaX + deltaY * deltaY;
			
			// Trigger the mouseClick-Event and mouseDoubleCLick!
			if( lastClickedGadget->isDoubleClickable() && this->framesSinceLastClick && this->framesSinceLastClick < maxDoubleClickPeriod && deltaTouchSqr < maxDoubleClickAreaSqr )
			{
				// Trigger the Event
				this->triggerEvent( event.setType( onMouseDblClick ) );
				
				// This prevents the already used onClick event from being part of another onDoubleClick event
				this->framesSinceLastClick = 0;
			}
			else{
				this->triggerEvent( event.setType( onMouseClick )  );	// Trigger the Event
				this->framesSinceLastClick = 1;							// Reset cycles since last click (0 would indicate that there was no last click)
			}
		}
		
		touchBefore = lastTouch; // Apply touch position for use in the next frame
		
		// Reset touch Cycles since the stylus is not pressed anymore
		touchCycles = 0;
	}
	
	return newTouchInside || isDragging;
}

_gadgetScreen::~_gadgetScreen(){
	_bitmap& bmp = _gadget::getBitmap();
	bmp.resetClippingRect();
	bmp.fill( _color::transparent );
}