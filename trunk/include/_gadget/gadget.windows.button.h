#ifndef _WIN_T_WINDOWSBUTTON_
#define _WIN_T_WINDOWSBUTTON_

#include "_gadget/gadget.button.h"
#include "_gadget/gadget.windows.startMenu.h"
#include "_gadget/gadget.window.h"

//
//  Startbutton
//

class _windowsStartButton : public _button {
	
	private:
	
		//! Images...
		static _bitmap	startButton;
		static _bitmap	startButtonPressed;
		
		_startMenu*		startMenu;
		
		static _callbackReturn  refreshHandler( _event );
		_callbackReturn  		mouseHandler( _event );
	
	public:
		
		_windowsStartButton( _coord x , _coord y , _style style = _style() );
};

//
// Taskbar-Window-Button
//

class _windowsTaskButton : public _button {
	
	private:
	
		//! The gadget (probaly a window) the taskBar-Button holds a reference to
		_window*	reference;
		
		static _callbackReturn  refreshHandler( _event );
		static _callbackReturn  mouseHandler( _event );
	
	public:
	
		//! Get Referenced Window
		_window* getReference(){ return this->reference; }
		
		_windowsTaskButton( _coord x , _coord y , _window* reference , _style style = _style() );
};

#endif