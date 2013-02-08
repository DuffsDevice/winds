#ifndef _WIN_T_WINDOWSBUTTON_
#define _WIN_T_WINDOWSBUTTON_

#include "_gadget/gadget.button.h"
#include "_gadget/gadget.windows.startmenu.h"

class _windowsStartButton : public _button {
	
	private:
		
		_startMenu*		startMenu;
		
		static _callbackReturn  refreshHandler( _event );
		_callbackReturn  		mouseHandler( _event );
	
	public:
		
		_windowsStartButton( _coord x , _coord y , _style style = _style() );
};

#endif