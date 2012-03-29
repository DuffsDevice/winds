// Check if already included
#ifndef _WIN_G_KEYBOARD_
#define _WIN_G_KEYBOARD_

#include "_type/type.gadget.h"
#include "_gadget/gadget.button.h"
#include <string>

using namespace std;

class _keyboardButton;

class _keyboard : public _gadget {
	
	private:
		
		//! Font of the Buttons
		_font* font;
		
		//! Internal Array for Keyboard-Layout
		static _rect buttonDimensions[];
		
		static _bitmap* bg;
		
		bool	shift;
		
		_gadget* destination;
		
		//! Label on the top of the button
		_keyboardButton* buttons[46];
		
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
		
		static _gadgetEventReturnType mouseHandler( _gadgetEvent event );
		
		static _gadgetEventReturnType keyHandler( _gadgetEvent event );
		
		void dtor();
		
	public:
		
		//! Set Receiver of Key-Events
		void setDestination( _gadget* dest ){ this->destination = dest; }
		
		//! Get Receiver of Key-Events
		_gadget* getDestination(){ return this->destination; }
		
		//! Constructor with dimsnions, coordinates, title and optional: Style
		_keyboard( _coord x , _coord y , _gadget* destination , _gadgetStyle style = _defaultStyle_ );
		
		//! Destrucor
		~_keyboard();
};


class _keyboardButton : public _button {
	
	private:
		
		_key key;
		
		static _gadgetEventReturnType  mouseHandler( _gadgetEvent );
		
		void init();
	
	public:
		
		_key getKey(){ return this->key; }
		
		void setKey( _key key ){ this->key = key; }
		
		_keyboardButton( _key key , _length width , _length height , _coord x , _coord y , string title , _gadgetStyle style = _defaultStyle_ )
			: _button( width , height , x , y , title , style ) , key( key )
		{ this->init(); }
		
		_keyboardButton( _key key , _coord x , _coord y , string text , _gadgetStyle style = _defaultStyle_ )
			: _button( x , y , text , style ) , key( key )
		{ this->init(); }
};

#endif