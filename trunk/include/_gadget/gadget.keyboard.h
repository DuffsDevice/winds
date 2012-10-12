// Check if already included
#ifndef _WIN_G_KEYBOARD_
#define _WIN_G_KEYBOARD_

#include "_type/type.gadgetScreen.h"
#include "_type/type.animation.h"
#include "_gadget/gadget.keyboard.button.h"

class _keyboardButton;

class _keyboard : public _gadgetScreen {
	
	private:
	
		_screen*	topScreen;
		_gadgetScreen*	gHScreen; // GadgetHost
		_u8			bgIdSub;
		int			magnifX;
		int			magnifY;
		float		magnifFactor;
		
		//! Font of the Buttons
		_font*		font;
		
		//! Internal Array for Keyboard-Layout
		static _rect buttonDimensions[];
		
		//! Images...
		static _bitmap* bg;
		
		//! State-Saver
		bool 	dragMe;
		bool	shift;
		bool	mode; // Whether the Keyboard is collapsed(=false) or expanded(=true)
		_s8		curState;
		
		//! Destination Gadget to receive Key-Events
		_gadget* destination;
		
		//! Animation
		_animation	animKeyb;
		_animation	animMagnif;
		
		//! Label on the top of the button
		_keyboardButton* buttons[46];
		
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
		static _gadgetEventReturnType mouseHandler( _gadgetEvent event );
		static _gadgetEventReturnType keyHandler( _gadgetEvent event );
		static _gadgetEventReturnType dragHandler( _gadgetEvent event );
		static _gadgetEventReturnType focusHandler( _gadgetEvent event );
		
		void setState( int val );
		void setMagnification( int val );
	
	public:
	
		//! Set Receiver of Key-Events
		void setDestination( _gadget* dest );
		
		//! Open!
		void open();
		
		//! Close
		void close();
		
		//! Check if opened
		bool isOpened(){ return this->mode; }
		
		//! Get Receiver of Key-Events
		_gadget* getDestination(){ return this->destination; }
		
		//! VBL of the Keyboard
		void screenVBL();
		
		//! Constructor with dimsnions, coordinates, title and optional: Style
		_keyboard( _u8 bgId , _gadgetScreen* gadgetHost , _screen* topScreen , _gadgetStyle style = _defaultStyle_ );
		
		//! Destrucor
		~_keyboard();
};

#endif