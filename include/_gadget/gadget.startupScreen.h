#ifndef _WIN_T_STARTUPSCREEN_H_
#define _WIN_T_STARTUPSCREEN_H_

#include "_type/type.h"
#include "_type/type.gadgetScreen.h"
#include "_type/type.system.h"

class _startupScreen : public _gadgetScreen
{
	private:
		
		_gadget* 		refresher;
		static _bitmap*	winLogo;
		_gadget*		winLogoGadget;
		
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
		
	public:
	
		_startupScreen( _u8 bgId , _gadgetStyle style = _gadgetStyle() );
		
		~_startupScreen();
	
};

#endif