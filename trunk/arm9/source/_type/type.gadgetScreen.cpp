#include "_type/type.gadgetScreen.h"
#include "_type/type.system.h"
#include <nds/arm9/background.h>

_gadgetScreen::_gadgetScreen( int bgId , _gadgetScreenType sType , _style&& style ) :
	_screen( bgId )
	, _gadget( _gadgetType::screen , 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT , _bitmap( _screen::getMemoryPtr() , SCREEN_WIDTH, SCREEN_HEIGHT ) , (_style&&)style )
	, screenType( sType )
	, touchCycles( 0 )
	, cyclesLastClick( 0 )
	, isDragging( false )
{
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


bool _gadgetScreen::processTouch( bool held , touchPosition origTouch )
{
	// Temp...
	_event event = _event( _none_ ).setCurrentKeys( _system::getCurrentKeys() );
	
	// Increase clickCycles and prevent overflow
	if( this->cyclesLastClick && !( this->cyclesLastClick >> 7 ) )
		this->cyclesLastClick++;
	
	_touch newTouch = origTouch;
	
	_touch  newNewTouch = _gadgetScreen::adjustTouch( newTouch );
	bool newTouchInside = this->touchInside( newNewTouch );
	
	//float cPR = 0.45f; // Stands for Cross-Panel-Resistance (á la Pressure)
	//if( origTouch.z1 && origTouch.z2 )
	//	float(origTouch.z2) / origTouch.z1;
	//cPR = 0.45f / cPR;
	//if( origTouch.z1 && origTouch.z2 )
	//{		
	//	cPR *= float( SCREEN_WIDTH - newTouch.x ) / 197.f + 1.f; // X-Correction
	//	_int sH2 = SCREEN_HEIGHT / 2;
	//	float xCorr = ( sH2 - abs( newTouch.y - sH2 ) ) / sH2;
	//	cPR *= 0.9f + xCorr * 0.1f; // Y-Correction
	//}
	//if( cPR < 0 )
	//	cPR = 0;
	//printf("Pressure: %f\n",cPR);
	
	/*!
	 * Handle the Pen!
	  */
	if( held )
	{
		event.setEffectivePosX( newTouch.x ).setEffectivePosY( newTouch.y ).setPosX( newNewTouch.x ).setPosY( newNewTouch.y );
		event.setPressure( 400 );
		
		//! Check if this is the first Cycle the pen is down
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
		
		//! Check if the Pen is dragging something
		if( this->isDragging )
		{
			// Adjust the lastTouch
			_touch newLastTouch = _gadgetScreen::adjustTouch( lastTouch );
			
			
			// Modify Event
			event.setDeltaX( newNewTouch.x - newLastTouch.x ).setDeltaY( newNewTouch.y - newLastTouch.y );
			
			
			// Trigger the Event
			this->triggerEvent( event.setType( onDragging ) );
			
			
			// Make ready for next frame!
			this->lastTouch = newTouch;
		}
		//! Apparently not dragging! -> Check if the Stylus has moved, but only if its start point was inside this screen
		else if( this->touchInside( newStartTouch ) )
		{
			//! Compute the dragged Distance: Pythagoras!
			_u32 xd = newTouch.x - startTouch.x;
			_u32 yd = newTouch.y - startTouch.y;
			_u32 dragDistance = xd * xd + yd * yd; // Actually its square
			_u8 trigger = _system::getUser().mDD;
			
			// For gadgets with a small dragging Trigger e.g. scrollBars
			if( _system::_lastClickedGadget_ && _system::_lastClickedGadget_->hasSmallDragTrig() )
				trigger /= 3;
			
			// Check if Pen has moved the distance already
			if( dragDistance > trigger * trigger )
			{
				// Modify Event
				event.setEffectivePosX( startTouch.x ).setEffectivePosY( startTouch.y ).setPosX( newStartTouch.x ).setPosY( newStartTouch.y );
				
				// start dragging
				this->triggerEvent( event.setType( onDragStart ) );
				this->isDragging = true;
				
				this->lastTouch = this->startTouch;
			}
			// If not dragging and If the current clicked gadget wants to have mouseClicks Repeating
			else if( _system::_lastClickedGadget_ && _system::_lastClickedGadget_->isMouseClickRepeat() && ( _system::getUser().kRD && this->touchCycles > _system::getUser().kRD && this->touchCycles % _system::getUser().kRS == 0 ) )
				this->triggerEvent( event.setType( onMouseRepeat ) );
		}
		
		// Increase Cycles
		this->touchCycles++;
		
	} // if( held ) {}
	
	//! Pen is not down
	else if( this->touchCycles > 0 )
	{
		//! Adjust the touch Position
		_touch newLastTouch = _gadgetScreen::adjustTouch( lastTouch );
		
		
		//! Set parameters
		event.setEffectivePosX( lastTouch.x ).setEffectivePosY( lastTouch.y ).setPosX( newLastTouch.x ).setPosY( newLastTouch.y ).setPressure(0);		
		
		
		// Temporary flag
		bool lastTouchInside = this->touchInside( newLastTouch );
		
		//! Trigger mouseUpEvent if the mouseUp was performed inside this screen
		if( lastTouchInside )
			this->triggerEvent( event.setType( onMouseUp ) );
		
		
		//! Maybe DragStop-Event?
		if( this->isDragging )
		{
			//! Trigger Event
			this->triggerEvent( event.setType( onDragStop ) );
			
			// Set touchDragging to false!
			this->isDragging = false;
		}
		//! Maybe Click-Event?
		else if( lastTouchInside && this->touchCycles < _system::getUser().mCC )
		{
			_s16	deltaX = touchBefore.x - lastTouch.x;
			_s16	deltaY = touchBefore.y - lastTouch.y;
			_length deltaTouch = deltaX * deltaX + deltaY * deltaY;
			
			
			// Trigger the mouseClick-Event and mouseDoubleCLick!
			if( _system::_lastClickedGadget_->isDoubleClickable() && this->cyclesLastClick && this->cyclesLastClick < _system::getUser().mDC && deltaTouch < pow( _system::getUser().mDA , 2 ) )
			{
				// Trigger the Event
				this->triggerEvent( event.setType( onMouseDblClick ) );
				
				this->cyclesLastClick = 0;
			}
			else
			{
				// Trigger the Event
				this->triggerEvent( event.setType( onMouseClick )  );
				
				this->cyclesLastClick = 1;
			}
		}
		
		// Apply touch position for use in the next frame
		touchBefore = lastTouch;
		
		// Reset touch Cycles
		touchCycles = 0;
	}
	
	return newTouchInside || isDragging;
}