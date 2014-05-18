#ifndef _WIN_S_BOOTUP_
#define _WIN_S_BOOTUP_

#include "_type/type.h"
#include "_type/type.screen.gadget.h"
#include "_gadget/gadget.progressbar.h"

class _bootupScreen : public _gadgetScreen
{
	private:
		
		_progressBar* 	bar;
		
		static _callbackReturn refreshHandler( _event );
		
	public:
	
		//! Ctor
		_bootupScreen( _u8 bgId , _style&& style = _style() );
		
		//! Dtor
		virtual ~_bootupScreen();
	
};

#endif