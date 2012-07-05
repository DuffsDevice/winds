#ifndef _WIN_T_GADGETSCREEN_
#define _WIN_T_GADGETSCREEN_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_type/type.screen.h"

class _gadgetScreen : public _gadget , public _screen
{
	private:
		
		_touch 	startTouch; // For Dragging
		_touch 	lastTouch;
		_touch 	touchBefore;
		_u32 	touchCycles;
		_u8 	cyclesLastClick;
		bool 	dragging;
	
	protected:
		
		_touch adjustTouch( _touch touch );
		
	public:
		
		//! Will be called every frame
		//! @return bool Whether the supplied touch was inside the range
		bool processTouch( bool held , _touch touch );
		
		//! C-tor
		_gadgetScreen( int bgId , _gadgetStyle style = _gadgetStyle() );
};

#endif