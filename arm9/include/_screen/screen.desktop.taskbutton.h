#ifndef _WIN_S_DESKTOP_TASKBUTTON_
#define _WIN_S_DESKTOP_TASKBUTTON_

#include <_gadget/gadget.button.h>
#include <_gadget/gadget.window.h>

class _desktopScreenTaskButton : public _button {
	
	private:
	
		//! The gadget (probaly a window) the taskBar-Button holds a reference to
		_window*	reference;
		
		static _callbackReturn  refreshHandler( _event );
		static _callbackReturn  mouseHandler( _event );
	
	public:
	
		//! Get Referenced Window
		_window* getReference(){ return this->reference; }
		
		_desktopScreenTaskButton( _optValue<_coord> x , _optValue<_coord> y , _window* reference , _style&& style = _style() );
};

#endif