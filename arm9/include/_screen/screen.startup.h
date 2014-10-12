#ifndef _WIN_S_STARTUP_
#define _WIN_S_STARTUP_

#include <_type/type.h>
#include <_type/type.screen.gadget.h>

class _startupScreen : public _gadgetScreen
{
	private:
		
		_gadget*			winLogoGadget;
		static _constBitmap	winLogo;
		
		static _callbackReturn refreshHandler( _event );
		
	public:
	
		//! Ctor
		_startupScreen( _u8 bgId , _style&& style = _style() );
		
		//! Dtor
		virtual ~_startupScreen(){
			delete this->winLogoGadget;
		}
	
};

#endif