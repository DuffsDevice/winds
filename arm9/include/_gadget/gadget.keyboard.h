// Check if already included
#ifndef _WIN_G_KEYBOARD_
#define _WIN_G_KEYBOARD_

#include "_type/type.gadget.screen.h"
#include "_type/type.animation.h"
#include "_type/type.sound.static.h"

class _keyboard : public _gadgetScreen {
	
	private:
	
		// Click Sound Object
		static _staticSound clickSound;
		
		_screen*			topScreen; // Sub-Scrern
		_gadgetScreen*		gHScreen; // GadgetHost-Screen
		
		//! Things'n'Stuff
		int					fromX;
		int					fromY;
		float				fromFactor;
		_s8					fromKeyboardExpansion;
		int					toX;
		int					toY;
		float				toFactor;
		_s8					toKeyboardExpansion;
		_s8					curState; // Variable to get the current state of the keyboard
		_optValue<_coord>	handlePosition;
		bool				manuallyOpened;
		bool				ignoreNextVBL;
		
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
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn updateHandler( _event );
		static _callbackReturn mouseHandler( _event );
		static _callbackReturn keyHandler( _event );
		static _callbackReturn dragHandler( _event );
		
		//! Setter for the animation and for the dragHandler
		void	setState( int val );
		
		//! Prepares 'anim' before a new animation starts
		void	prepareAnimation();
	
	public:
	
		//! Method to be invoked every 1/60s
		void vbl();
		
		//! Open!
		void open( bool useAnim = true );
		
		//! Resets the zoom in opened state
		void resetZoom( bool useAnim = true );
		
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
			this->update();
		}
		
		//! Set Shift on ('true') or off ('false')
		void setShift( bool shift = true ){
			this->shift = shift;
			this->update();
		}
		
		//! Plays a click sound
		static void playClickSound();
		
		//! Ctor
		_keyboard( _u8 bgId , _gadgetScreen* gadgetHost , _screen* topScreen , _optValue<_coord> handlePosition = ignore , _style&& style = _style::canNotTakeFocus );
		
		//! Dtor
		~_keyboard();
};

#endif