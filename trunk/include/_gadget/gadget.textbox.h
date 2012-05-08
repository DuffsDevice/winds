// Check if already included
#ifndef _WIN_G_TEXTBOX_
#define _WIN_G_TEXTBOX_

#include "_type/type.gadget.h"
#include "_gadget/gadget.label.h"
#include "interface.input.h"
#include <string>

using namespace std;

class _textbox : public _label {
	
	private:
		
		_length	cursor;
		
		bool	pressed;
		
		static _gadgetEventReturnType refreshHandler( _gadgetEvent e );
		static _gadgetEventReturnType focusHandler( _gadgetEvent e );
		static _gadgetEventReturnType blurHandler( _gadgetEvent e );
		static _gadgetEventReturnType mouseHandler( _gadgetEvent e );
		static _gadgetEventReturnType keyHandler( _gadgetEvent e );
		
	public:
		
		//! Construcor excluding dimensions and including font
		_textbox( _coord x , _coord y , _length width , string value , _gadgetStyle style = _defaultStyle_ );
		
		//! Destructor
		~_textbox();
};

#endif