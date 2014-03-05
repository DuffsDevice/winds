#ifndef _WIN_T_STARTUPSCREEN_H_
#define _WIN_T_STARTUPSCREEN_H_

#include "_type/type.h"
#include "_type/type.gadget.screen.h"

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
		~_startupScreen(){
			delete this->winLogoGadget;
		}
	
};

#endif