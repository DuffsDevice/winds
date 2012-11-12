// Check if already included
#ifndef _WIN_G_TEXTBOX_
#define _WIN_G_TEXTBOX_

#include "_type/type.gadget.h"
#include "_gadget/gadget.label.h"
#include "interface.input.h"

class _textbox : public _label {
	
	private:
		
		_length	cursor;
		
		bool	pressed;
		
		static _callbackReturn refreshHandler( _event e );
		static _callbackReturn focusHandler( _event e );
		static _callbackReturn blurHandler( _event e );
		static _callbackReturn mouseHandler( _event e );
		static _callbackReturn keyHandler( _event e );
		
	public:
		
		//! Construcor excluding dimensions and including font
		_textbox( _coord x , _coord y , _length width , string value , _style style = _style() );
};

#endif