#ifndef _WIN_T_WINDOWSBUTTON_
#define _WIN_T_WINDOWSBUTTON_

#include "_gadget/gadget.button.h"
#include "_gadget/gadget.windows.startMenu.h"

class _windowsStartButton : public _button {
	
	private:
	
		//! Images...
		static _bitmap* startButton;
		static _bitmap* startButtonPressed;
		
		_startMenu*		startMenu;
		
		static _gadgetEventReturnType  refreshHandler( _gadgetEvent );
		
		static _gadgetEventReturnType  mouseHandler( _gadgetEvent );
	
	public:
		
		_windowsStartButton( _coord x , _coord y , _gadgetStyle style = _defaultStyle_ );
};

#endif