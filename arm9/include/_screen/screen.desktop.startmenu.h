#ifndef _WIN_S_DESKTOP_STARTMENU_
#define _WIN_S_DESKTOP_STARTMENU_

#include <_type/type.gadget.h>
#include <_gadget/gadget.popup.h>
#include <_gadget/gadget.label.h>
#include <_gadget/gadget.button.action.h>

class _desktopScreenStartMenu : public _popup{
	
	private:
	
		static _callbackReturn refreshHandler( _event );
		_callbackReturn clickHandler( _event );
		_callbackReturn buttonHandler( _event );
		
		_label*			shutdownLabel;
		_label*			logOffLabel;
		_actionButton*	shutdownButton;
		_actionButton*	logOffButton;
	
	public:
	
		//! Ctor
		_desktopScreenStartMenu( _style&& style = _style() );
		
		//! Dtor
		~_desktopScreenStartMenu();

};	

#endif