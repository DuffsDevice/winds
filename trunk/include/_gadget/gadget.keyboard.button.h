#ifndef _WIN_T_KEYBOARDBUTTON_
#define _WIN_T_KEYBOARDBUTTON_

#include "_gadget/gadget.button.h"
#include "_gadget/gadget.windows.startMenu.h"

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

class _keyboardStartButton : public _button {
	
	private:
	
		//! Images...
		static _bitmap* startButton;
		static _bitmap* startButtonPressed;
		
		_startMenu*		startMenu;
		
		static _gadgetEventReturnType  refreshHandler( _gadgetEvent );
		
		static _gadgetEventReturnType  mouseHandler( _gadgetEvent );
	
	public:
		
		_keyboardStartButton( _coord x , _coord y , _gadgetStyle style = _defaultStyle_ );
};

#endif