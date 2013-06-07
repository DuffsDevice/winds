// Check if already included
#ifndef _WIN_G_KEYBOARD_
#define _WIN_G_KEYBOARD_

#include "_type/type.gadgetScreen.h"
#include "_type/type.animation.h"

class _keyboard : public _gadgetScreen {
	
	private:
	
		_screen*		topScreen; // Sub-Scrern
		_gadgetScreen*	gHScreen; // GadgetHost-Screen
		
		//! Things'n'Stuff
		int				fromX;
		int				fromY;
		float			fromFactor;
		_s8				fromKeyboardExpansion;
		int				toX;
		int				toY;
		float			toFactor;
		_s8				toKeyboardExpansion;
		_s8				curState; // Variable to get the current state of the keyboard
		_coord			handlePosition;
		
		//! Internal Array for Keyboard-Layout
		static _rect 	buttonDimensions[];
		
		//! Images...
		static _bitmap* bg;
		
		//! State-Saver
		bool			dragMe;
		bool			shift;
		bool			caps;
		bool			mode; // Whether the Keyboard is collapsed(=false) or expanded(=true)
		
		//! Destination Gadget to receive Key-Events
		static _gadget*	lastCurrentFocus; // Temp-variable to tell if the '_system::_currentFocus_' variable has changed
		
		//! Animation
		_animation		anim;
		
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn mouseHandler( _event event );
		static _callbackReturn keyHandler( _event event );
		static _callbackReturn dragHandler( _event event );
		
		//! Setter for the animation and for the dragHandler
		int		setState( int val );
		
		//! Prepares 'anim' before a new animation starts
		void	prepareAnimation();
		
		//! Update the keyboard-buttons
		void	refreshKeys();
	
	public:
		
		//! Open!
		void open( bool useAnim = true );
		
		//! Close
		void close( bool useAnim = true );
		
		//! Check if opened
		bool isOpened(){ return this->mode; }
		
		//! Check if either shift or capslock is enabled
		bool isShift(){ return this->shift; }
		bool isCaps(){ return this->caps; }
		
		//! Set Capslock on ('true') or off ('false')
		void setCaps( bool caps = true ){
			this->caps = caps;
			refreshKeys();
		}
		
		//! Set Shift on ('true') or off ('false')
		void setShift( bool shift = true ){
			this->shift = shift;
			refreshKeys();
		}
		
		//! VBL of the Keyboard
		void screenVBL();
		
		//! Ctor
		_keyboard( _u8 bgId , _gadgetScreen* gadgetHost , _screen* topScreen , _coord handlePosition = ( SCREEN_WIDTH - 40 ) , _style style = _style() | _styleAttr::canNotTakeFocus );
		
		//! Dtor
		~_keyboard();
};

#endif