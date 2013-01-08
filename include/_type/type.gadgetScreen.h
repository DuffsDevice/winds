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

class _gadgetScreen : public _gadget , public _screen
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
		
		_touch adjustTouch( _touch touch );
		
	public:
		
		//! Will be called every frame
		//! @return bool Whether the supplied touch was inside the range
		bool processTouch( bool held , _touch touch );
		
		//! Get the Meaning of the screen (what is it supposed to do)
		_gadgetScreenType getScreenType(){ return this->screenType; }
		
		//! Dimensions for gadgets that are maximized
		virtual _rect getMaximizedDimensions(){ return _rect(); }
		
		//! C-tor
		_gadgetScreen( int bgId , _gadgetScreenType sType , _style style = _style() );
};

#endif