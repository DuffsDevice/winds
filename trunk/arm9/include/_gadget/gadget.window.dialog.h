// Check if already included
#ifndef _WIN_G_WINDOW_DIALOG_
#define _WIN_G_WINDOW_DIALOG_

#include "_gadget/gadget.window.h"

class _dialogWindow : public _window
{
	public:
		
		//! Handles Submit and Escape-Keys
		static _callbackReturn submitEscapeHandler( _event );
		
		//! Ctor
		_dialogWindow( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string title , _style&& style = _style() );
};

#endif