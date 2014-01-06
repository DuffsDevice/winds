#ifndef _WIN_G_STARTMENU_
#define _WIN_G_STARTMENU_

#include "_type/type.gadget.h"
#include "_gadget/gadget.popup.h"

class _startMenu : public _popup{
	
	private:
	
		static _callbackReturn refreshHandler( _event );
	
	public:
	
		//! Ctor
		_startMenu( _style&& style = _style() );
		
		//! Dtor
		~_startMenu();

};	

#endif