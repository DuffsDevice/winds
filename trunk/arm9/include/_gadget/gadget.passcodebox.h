#ifndef _WIN_G_PASSCODEBOX_
#define _WIN_G_PASSCODEBOX_

#include "_gadget/gadget.textbox.h"

class _passcodeBox : public _textBox
{
	private:
		
		//! The real value of the passcode box
		string			realText;
		
		//! Char that will be used as replacement
		_char 			replaceChar;
		const _font*	replaceFont;
		
		//! Internal
		void refresher();
		
		//! Override of _textBox::(..)
		void removeStr( _int position , _length numChars = 1 );
		void insertStr( _int position , string s );
		
	public:
		
		//! Set string-value
		void setStrValue( string val );
		
		//! Get string-value
		string getStrValue(){ return this->realText; }
		
		//! Ctor
		_passcodeBox( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string value = "" , const _font* replaceFont = nullptr , _char replaceChar = '+' , _style&& style = _style() );
};

#endif