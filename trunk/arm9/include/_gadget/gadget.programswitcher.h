#ifndef _WIN_G_PROGRAMSWITCHER_
#define _WIN_G_PROGRAMSWITCHER_

#include <_type/type.h>
#include <_type/type.gadget.h>
#include <_type/type.program.handle.h>
#include <_gadget/gadget.window.h>
#include <_gadget/gadget.popup.h>
#include <_gadget/gadget.label.h>

class _programSwitcher : public _popup
{
	
	private:
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn keyHandler( _event );
		static _callbackReturn updateHandler( _event );
		static _callbackReturn closeHandler( _event );
		
		void taskChangeHandler( _window* ref );
		
		_label* label;
		
	public:
		
		//! Ctor
		_programSwitcher( _style&& style = _style() );
		
		//! Dtor
		~_programSwitcher();
};

#endif