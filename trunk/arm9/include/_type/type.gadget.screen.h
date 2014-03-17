#ifndef _WIN_T_GADGETSCREEN_
#define _WIN_T_GADGETSCREEN_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_type/type.screen.h"

enum class _gadgetScreenType : _u8
{
	windows,
	startUp,
	bootUp,
	keyboard
};

class _gadgetScreen : public _screen , public _gadget
{
	private:
		
		_gadgetScreenType	screenType;
		_touch 				startTouch; // For Dragging
		_touch 				lastTouch;
		_touch 				touchBefore;
		_u32 				touchCycles;
		_u8 				cyclesLastClick;
		bool 				isDragging;
	
	protected:
		
		// Adjust supplied _touch so that it is relative to the current position of the screen
		_touch adjustTouch( _touch touch );
		
		// Check whether the supplied touch position is inside this screen
		bool touchInside( _touch touch );
		
	public:
		
		// Make available _screen::screen instead of the mothod from _gadget
		using _screen::getBitmap;
		
		//! Will be called every frame
		//! @return bool Whether the supplied touch was inside the range
		bool processTouch( bool held , _touch touch );
		
		//! Get the Meaning of the screen (what is it supposed to do)
		_gadgetScreenType getScreenType(){ return this->screenType; }
		
		//! C-tor
		_gadgetScreen( int bgId , _gadgetScreenType sType , _style&& style = _style() );
};

#endif