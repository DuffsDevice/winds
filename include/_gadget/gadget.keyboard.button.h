#ifndef _WIN_T_KEYBOARDBUTTON_
#define _WIN_T_KEYBOARDBUTTON_

#include "_gadget/gadget.button.h"
#include "_gadget/gadget.windows.startMenu.h"

class _keyboardButton : public _button {
	
	private:
		
		//! the Key it represents (e.g. upper case 'A')
		/*! This Key will be sent in the keyClick-event as 'keyCode' */
		_key key;
		
		//! Used to throw an keyClick-event
		static _callbackReturn  mouseHandler( _event );
		
		static _callbackReturn  refreshHandler( _event );
	
	public:
		
		//! Get the Key it represents (e.g. upper case 'A')
		_key getKey(){ return this->key; }
		
		//! Set The key it represents
		void setKey( _key key ){ this->key = key; }
		
		//! Ctor
		_keyboardButton( _key key , _optValue<_length> width , _optValue<_length> height , _coord x , _coord y 
			, string title , _style&& style = _style() | _styleAttr::mouseClickRepeat );
};

#endif