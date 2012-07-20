#include "_type/type.gadgetScreen.h"
#include "_type/type.system.h"

#include <nds/arm9/console.h>

_gadgetScreen::_gadgetScreen( int bgId , _gadgetStyle style ) :
	_gadget( _gadgetType::screen , SCREEN_WIDTH , SCREEN_HEIGHT , 0 , 0 , style , true )
	, _screen( bgId )
	, touchCycles( 0 )
	, cyclesLastClick( 0 )
	, dragging( false )
{
	//! Allocate bitmap using the base Memory of the Background-Layer
	this->bitmap = new _bitmap( _screen::getMemoryPtr() , SCREEN_WIDTH, SCREEN_HEIGHT );
}

_touch _gadgetScreen::adjustTouch( _touch touch )
{
	touch.x = int( float(touch.x) * this->getScaleX() ) + this->getScrollX();
	touch.y = int( float(touch.y) * this->getScaleY() ) + this->getScrollY();
	return touch;
}

bool _gadgetScreen::processTouch( bool held , _touch newTouch )
{
	// Shortcut...
	const _user* user = _system_->_runtimeAttributes_->user;
	
	// Temp...
	_gadgetEventArgs args;
	
	// Increase clickCycles and prevent overflow
	if( this->cyclesLastClick && !( this->cyclesLastClick >> 7 ) )
		this->cyclesLastClick++;
	
	_touch  newNewTouch = _gadgetScreen::adjustTouch( newTouch );
	bool newTouchInside = _rect( 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT ).contains( newNewTouch.x , newNewTouch.y ) && RGB_GETA( this->getMemoryPtr()[SCREEN_WIDTH*newNewTouch.y+newNewTouch.x] );
	
	/*!
	 * Handle the Pen!
	 **/
	if( held )
	{
		args.reset();
		args.setEffectivePosX( newTouch.x );
		args.setEffectivePosY( newTouch.y );
		args.setPosX( newNewTouch.x );
		args.setPosY( newNewTouch.y );
			
		//! Check if this is the first Cycle the pen is down
		if( this->touchCycles == 0 )
		{
			// Trigger the Event
			if( newTouchInside )
				this->handleEvent( _gadgetEvent( "mouseDown" , args ) );
			
			// Set the starting Point of the mouseDown here!
			this->startTouch = newTouch;
			this->lastTouch = newTouch;
		}
		
		_touch newStartTouch = _gadgetScreen::adjustTouch( startTouch );
		
		//! Check if the Pen is dragging something
		if( this->dragging )
		{
			// Trigger the Event
			_touch newLastTouch = _gadgetScreen::adjustTouch( lastTouch );
			args.setPosX( newLastTouch.x );
			args.setPosY( newLastTouch.y );
			args.setDeltaX( newNewTouch.x - newLastTouch.x );
			args.setDeltaY( newNewTouch.y - newLastTouch.y );
			
			this->handleEvent( _gadgetEvent( "dragging" , args ) );
			
			this->lastTouch = newTouch;
		}		
		//! Check if the Stylus has moved
		else if( _rect( 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT ).contains( newStartTouch.x , newStartTouch.y ) && RGB_GETA( this->getMemoryPtr()[SCREEN_WIDTH*newStartTouch.y+newStartTouch.x] ) )
		{
			// Compute the dragged Distance
			// -> Pythagoras!
			u32 xd = newTouch.x - startTouch.x;
			u32 yd = newTouch.y - startTouch.y;
			u32 dragDistance = xd * xd + yd * yd;
			
			// Check if Pen has moved the distance already
			if( dragDistance > user->mDD * user->mDD )
			{
				// Set the Args
				args.reset();
				args.setEffectivePosX( startTouch.x );
				args.setEffectivePosY( startTouch.y );
				args.setPosX( newStartTouch.x );
				args.setPosY( newStartTouch.y );
				
				// start dragging
				this->handleEvent( _gadgetEvent( "dragStart" , args ) );
				dragging = true;
				
				this->lastTouch = this->startTouch;	
			}
		}
		
		
		// Increase Cycles
		this->touchCycles++;
	}
	//! Pen is not down
	else if( this->touchCycles > 0 )
	{
		// Set the Args
		args.reset();
		args.setEffectivePosX( lastTouch.x );
		args.setEffectivePosY( lastTouch.y );
		_touch newLastTouch = _gadgetScreen::adjustTouch( lastTouch );
		args.setPosX( newLastTouch.x );
		args.setPosY( newLastTouch.y );
		
		
		//! Maybe DragStop-Event?
		if( this->dragging )
		{
			//! Trigger Event
			this->handleEvent( _gadgetEvent( "dragStop" , args ) );
			
			// Set touchDragging to false!
			this->dragging = false;
		}
		//! Maybe Click-Event?
		else if( this->touchCycles < user->mCC )
		{
			_s16	deltaX = touchBefore.x - lastTouch.x;
			_s16	deltaY = touchBefore.y - lastTouch.y;
			_s16 	deltaTouch = deltaX * deltaX + deltaY * deltaY;
			
			// Trigger the mouseClick-Event and mouseDoubleCLick!
			if( this->cyclesLastClick && this->cyclesLastClick < user->mDC && deltaTouch < user->mDA * user->mDA )
			{
				// Trigger the Event
				this->handleEvent( _gadgetEvent( "mouseDoubleClick" , args ) );
				this->cyclesLastClick = 0;
			}
			else
			{
				// Trigger the Event
				this->handleEvent( _gadgetEvent( "mouseClick" , args ) );
				
				this->cyclesLastClick = 1;
			}
		}
		
		// Trigger the Event
		this->handleEvent( _gadgetEvent( "mouseUp" , args ) );
		
		touchBefore = lastTouch;
		
		// Reset touch Cycles
		touchCycles = 0;
		
	}
	
	return newTouchInside || dragging;
}