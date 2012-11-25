#ifndef _WIN_T_WINDOWSBUTTON_
#define _WIN_T_WINDOWSBUTTON_

#include "_gadget/gadget.button.h"
#include "_gadget/gadget.windows.startMenu.h"

class _windowsStartButton : public _button {
	
	private:
	
		//! Images...
		static _bitmap	startButton;
		static _bitmap	startButtonPressed;
		
		_startMenu*		startMenu;
		
		static _callbackReturn  refreshHandler( _event );
		
		static _callbackReturn  mouseHandler( _event );
	
	public:
		
		_windowsStartButton( _coord x , _coord y , _style style = _style() );
};

#endif