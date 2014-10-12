#ifndef _WIN_S_DESKTOP_STARTMENU_
#define _WIN_S_DESKTOP_STARTMENU_

#include <_type/type.gadget.h>
#include <_gadget/gadget.popup.h>

class _desktopScreenStartMenu : public _popup{
	
	private:
	
		static _callbackReturn refreshHandler( _event );
		_callbackReturn clickHandler( _event );
	
	public:
	
		//! Ctor
		_desktopScreenStartMenu( _style&& style = _style() );
		
		//! Dtor
		~_desktopScreenStartMenu();

};	

#endif