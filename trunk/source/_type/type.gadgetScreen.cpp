#include "_type/type.gadgetScreen.h"
#include "_type/type.system.h"

_gadgetScreen::_gadgetScreen( int bgId , _gadgetScreenType sType , _style style ) :
	_gadget( _gadgetType::screen , SCREEN_WIDTH , SCREEN_HEIGHT , 0 , 0 , style , true )
	, _screen( bgId )
	, screenType( sType )
	, touchCycles( 0 )
	, cyclesLastClick( 0 )
	, isDragging( false )
{
	//! Allocate bitmap using the base Memory of the Background-Layer
	this->bitmap = _bitmap( _screen::getMemoryPtr() , SCREEN_WIDTH, SCREEN_HEIGHT );
}

_touch _gadgetScreen::adjustTouch( _touch touch )
{
	touch.x = int( float(touch.x) * this->getScaleX() ) + this->getScrollX();
	touch.y = int( float(touch.y) * this->getScaleY() ) + this->getScrollY();
	return touch;
}

bool _gadgetScreen::touchInside( _touch touch )
{
	return _rect( 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT ).contains( touch.x , touch.y ) // Inside my coordinates
		&& RGB_GETA( _gadget::getBitmap().getPixel( touch.x , touch.y ) ); // Check if this part of the bitmap is non-transparent
}

bool _gadgetScreen::processTouch( bool held , _touch newTouch )
{
	// Shortcut...
	const _user* user = _system::_runtimeAttributes_->user;
	
	// Temp...
	_event event = _event().setCurrentKeyCodes( _system::getCurrentKeys() );
	
	// Increase clickCycles and prevent overflow
	if( this->cyclesLastClick && !( this->cyclesLastClick >> 7 ) )
		this->cyclesLastClick++;
	
	_touch  newNewTouch = _gadgetScreen::adjustTouch( newTouch );
	bool newTouchInside = this->touchInside( newNewTouch );
	
	/*!
	 * Handle the Pen!
	 **/
	if( held )
	{
		
		event.setEffectivePosX( newTouch.x ).setEffectivePosY( newTouch.y ).setPosX( newNewTouch.x ).setPosY( newNewTouch.y );
			
		//! Check if this is the first Cycle the pen is down
		if( this->touchCycles == 0 )
		{
			// Trigger the Event
			if( newTouchInside )
				this->handleEvent( event.setType( mouseDown ) );
			
			// Set the starting Point of the mouseDown here!
			this->startTouch = newTouch;
			this->lastTouch = newTouch;
		}
		
		_touch newStartTouch = _gadgetScreen::adjustTouch( startTouch );
		
		//! Check if the Pen is dragging something
		if( this->isDragging )
		{
			// Trigger the Event
			_touch newLastTouch = _gadgetScreen::adjustTouch( lastTouch );
			
			// Modify Event
			event.setDeltaX( newNewTouch.x - newLastTouch.x ).setDeltaY( newNewTouch.y - newLastTouch.y );
			
			//! '_gadget' shall pass the 'Drag'-Event
			this->handleEvent( event.setType( dragging ) );
			
			//! Make ready for next frame!
			this->lastTouch = newTouch;
		}		
		//! Check if the Stylus has moved
		else if( this->touchInside( newStartTouch ) ){
			// Compute the dragged Distance
			// -> Pythagoras!
			_u32 xd = newTouch.x - startTouch.x;
			_u32 yd = newTouch.y - startTouch.y;
			_u32 dragDistance = xd * xd + yd * yd;
			_u8 trigger = user->mDD;
			
			// For gadgets with a small dragging Trigger e.g. scrollBars
			if( _system::_lastClickedGadget_ && _system::_lastClickedGadget_->hasSmallDragTrig() )
				trigger /= 3;
			
			// Check if Pen has moved the distance already
			if( dragDistance > trigger * trigger )
			{
				// Modify Event
				event.setEffectivePosX( startTouch.x ).setEffectivePosY( startTouch.y ).setPosX( newStartTouch.x ).setPosY( newStartTouch.y );
				
				// start dragging
				this->handleEvent( event.setType( dragStart ) );
				this->isDragging = true;
				
				this->lastTouch = this->startTouch;	
			}
		}
		
		// If the current clicked gadget wants to have mouseClicks Repeating
		if( _system::_lastClickedGadget_ && _system::_lastClickedGadget_->isMouseClickRepeat() && ( user->kRD && this->touchCycles > user->kRD && this->touchCycles % user->kRS == 0 ) )
			_system::_lastClickedGadget_->handleEvent( event.setType( mouseClick ) );
		
		// Increase Cycles
		this->touchCycles++;
	}
	//! Pen is not down
	else if( this->touchCycles > 0 )
	{
		// Set the Event-Parameter
		_touch newLastTouch = _gadgetScreen::adjustTouch( lastTouch );
		
		// Set parameters
		event.setEffectivePosX( lastTouch.x ).setEffectivePosY( lastTouch.y ).setPosX( newLastTouch.x ).setPosY( newLastTouch.y );		
		
		bool lastTouchInside = this->touchInside( newLastTouch );
		
		// Trigger mouseUpEvent if the mouseUp was performed inside this screen
		if( lastTouchInside )
			this->handleEvent( event.setType( mouseUp ) );
		
		//! Maybe DragStop-Event?
		if( this->isDragging )
		{
			//! Trigger Event
			this->handleEvent( event.setType( dragStop ) );
			
			// Set touchDragging to false!
			this->isDragging = false;
		}
		//! Maybe Click-Event?
		else if( lastTouchInside && this->touchCycles < user->mCC )
		{
			_s16	deltaX = touchBefore.x - lastTouch.x;
			_s16	deltaY = touchBefore.y - lastTouch.y;
			_length deltaTouch = deltaX * deltaX + deltaY * deltaY;
			
			// Trigger the mouseClick-Event and mouseDoubleCLick!
			if( this->cyclesLastClick && this->cyclesLastClick < user->mDC && deltaTouch < user->mDA * user->mDA )
			{
				// Trigger the Event
				this->handleEvent( event.setType( mouseDoubleClick ) );
				this->cyclesLastClick = 0;
			}
			else
			{
				// Trigger the Event
				this->handleEvent( event.setType( mouseClick )  );
				
				this->cyclesLastClick = 1;
			}
		}
		
		touchBefore = lastTouch;
		
		// Reset touch Cycles
		touchCycles = 0;
	}
	
	return newTouchInside || isDragging;
}