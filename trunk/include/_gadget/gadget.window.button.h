#ifndef _WIN_G_WINDOWBUTTON_
#define _WIN_G_WINDOWBUTTON_

#include "_type/type.gadget.h"
#include "_gadget/gadget.button.h"

class _windowButton : public _button
{
	private:
		
		//! Specifies the Type of the button (close-button, minimize/restore-button etc...)
		_u8 buttonType;
		
		//! Called upon refresh events
		static _callbackReturn refreshHandler( _event event );
		
	public:
		
		//! Ctor
		_windowButton( _coord x , _coord y , _u8 buttonType );
};

#endif