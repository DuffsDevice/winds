#ifndef _WIN_G_PASSCODEBOX_
#define _WIN_G_PASSCODEBOX_

#include <_gadget/gadget.textbox.h>

class _passcodeBox : public _textBox
{
	private:
		
		//! The real value of the passcode box
		wstring			realText;
		
		//! Char that will be used as replacement
		_char 			replaceChar;
		
		//! Internal
		void refresher();
		
		//! Override of _textBox::(..)
		void removeStr( _int position , _length numChars = 1 );
		void insertStr( _int position , wstring s );
		
	public:
		
		//! Set string-value
		void setStrValue( wstring val );
		
		//! Get string-value
		wstring getStrValue(){ return this->realText; }
		
		//! Ctor
		_passcodeBox( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , wstring value = "" , _optValue<_char> replaceChar = ignore , _style&& style = _style() );
};

#endif