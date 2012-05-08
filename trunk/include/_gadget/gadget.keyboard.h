// Check if already included
#ifndef _WIN_G_KEYBOARD_
#define _WIN_G_KEYBOARD_

#include "_type/type.gadget.h"
#include "_type/type.animation.h"
#include "_gadget/gadget.keyboard.button.h"
#include <string>

using namespace std;

class _keyboardButton;

class _keyboard : public _gadget {
	
	private:
	
		//! Font of the Buttons
		_font* font;
		
		//! Internal Array for Keyboard-Layout
		static _rect buttonDimensions[];
		
		//! Images...
		static _bitmap* bg;
		
		_keyboardStartButton*	startButton;
		
		//! State-Saver
		bool 	dragMe;
		bool	shift;
		bool	mode; // Whether the Keyboard is collapsed(=false) or expanded(=true)
		
		//! Destination Gadget to receive Key-Events
		_gadget* destination;
		
		//! Animation
		_animation	anim;
		
		//! Label on the top of the button
		_keyboardButton* buttons[46];
		
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
		static _gadgetEventReturnType mouseHandler( _gadgetEvent event );
		static _gadgetEventReturnType keyHandler( _gadgetEvent event );
		static _gadgetEventReturnType dragHandler( _gadgetEvent event );
	
	public:
	
		//! Set Receiver of Key-Events
		void setDestination( _gadget* dest );
		
		//! Get Receiver of Key-Events
		_gadget* getDestination(){ return this->destination; }
		
		//! Constructor with dimsnions, coordinates, title and optional: Style
		_keyboard( _gadgetStyle style = _defaultStyle_ );
		
		//! Destrucor
		~_keyboard();
};

#endif