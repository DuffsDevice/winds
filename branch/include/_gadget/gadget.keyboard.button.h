#ifndef _WIN_T_KEYBOARDBUTTON_
#define _WIN_T_KEYBOARDBUTTON_

#include "_gadget/gadget.button.h"

class _keyboardButton : public _button {
	
	private:
		
		_key key;
		
		static _gadgetEventReturnType  mouseHandler( _gadgetEvent );
		
		void init();
	
	public:
		
		_key getKey();
		
		void setKey( _key key );
		
		_keyboardButton( _key key , _length width , _length height , _coord x , _coord y , string title , _gadgetStyle style = _defaultStyle_ );
		
		_keyboardButton( _key key , _coord x , _coord y , string text , _gadgetStyle style = _defaultStyle_ );
};

#endif