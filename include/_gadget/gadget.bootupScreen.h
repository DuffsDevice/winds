#ifndef _WIN_T_BOOTUPSCREEN_H_
#define _WIN_T_BOOTUPSCREEN_H_

#include "_type/type.h"
#include "_type/type.gadgetScreen.h"
#include "_gadget/gadget.progressbar.h"
#include "_type/type.system.h"

class _bootupScreen : public _gadgetScreen
{
	private:
		
		_gadget* 		refresher;
		_progressbar* 	bar;
		
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
		
	public:
	
		_bootupScreen( _u8 bgId , _gadgetStyle style = _gadgetStyle() );
		
		~_bootupScreen();
	
};

#endif