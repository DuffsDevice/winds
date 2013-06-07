#ifndef _WIN_G_PASSCODEBOX_
#define _WIN_G_PASSCODEBOX_

#include "_gadget/gadget.textbox.h"

class _passcodebox : public _textbox
{
	private:
		
		//! The real value of the passcode box
		string			realText;
		
		//! Char that will be used as replacement
		_char 			replaceChar;
		const _font*	replaceFont;
		
		//! Internal
		void refresher();
		
		//! Override of _textbox::(..)
		void removeStr( _int position , _length numChars = 1 );
		void insertStr( _int position , string s );
		
	public:
		
		//! Set string-value
		void setStrValue( string val );
		
		//! Get string-value
		string getStrValue(){ return this->realText; }
		
		//! Ctor
		_passcodebox( _coord x , _coord y , _length width , string value = "" , const _font* replaceFont = nullptr , _char replaceChar = '+' , _style style = _style() );
		
		//! Ctor
		_passcodebox( _coord x , _coord y , _length width , _length height , string value = "" , const _font* replaceFont = nullptr , _char replaceChar = '+' , _style style = _style() );
};

#endif