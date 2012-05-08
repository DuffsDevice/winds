#ifndef _WIN_T_GADGETEVENTARGS_
#define _WIN_T_GADGETEVENTARGS_

#include "type.h"
#include "type.rect.h"
#include <stdio.h>

/**
 * Specifies the Type of an Event
**/
class _gadgetEventArgs
{
	private:
		
		void* 	src;			 //! Triggering Gadget not defined sometimes
		void* 	dest;			 //! Gadget to receive the Event
		_coord 	posX;			 //! X-Position of the Mouse when the Event was triggered
		_coord 	posY;			 //! Y-Position of the Mouse when the Event was triggered
		_coord 	deltaX;			 //! Delta-X, used at dragging-events
		_coord 	deltaY;			 //! Delta-Y, used at dragging-events
		_key 	keyCode;		 //! KeyCode of the Button that triggered the Event
		_key 	currentKeyCodes; //! Keycode-State of that Moment the Event was triggered
		_u32	heldTime; 		 //! Time the Button was Pressed (only for: keyUp, keyClick, mouseUp, mouseClick )
		_area	damagedRects;	 //! If (isBubbleRefresh) is true then this Attribute will specify the Area, that is invalid/damaged and has to be repainted
		bool	bubble;	 		 //! If 'true', all events needed for informing the parents are already thrown, no need to throw the again
		
	public:
		
		//! Constructors
		_gadgetEventArgs( void* dest = nullptr );
		
		//! Reset All Arguments
		void reset();
		
		//! Get Destination Gadget
		void* getDestination();
		
		//! Get Trigger Gadget
		void* getSource();
		
		//! Set the Destination
		void setDestination( void* newVal );
		
		//! Set the Source
		void setSource( void* newVal );
		
		//! Get Triggering Point X
		_coord getPosX();
		
		//! Get Triggering Point Y
		_coord getPosY();
		
		//! Set Triggering Point X
		void setPosX( _coord pos );
		
		//! Set Triggering Point Y
		void setPosY( _coord pos );
		
		//! Get Delta-X
		_coord getDeltaX();
		
		//! Get Delta-Y
		_coord getDeltaY();
		
		//! Set Triggering Point X
		void setDeltaX( _coord pos );
		
		//! Set Triggering Point Y
		void setDeltaY( _coord pos );
		
		//! Get triggering KeyCode
		_key getKeyCode();
		
		//! Set triggering KeyCode
		void setKeyCode( _key code );
		
		//! Get Held Time of the key that triggered the Event
		_u32 getHeldTime();
		
		//! Set Held Time of the key that triggered the Event
		void setHeldTime( _u32 heldTime );
		
		//! Get KeyCode State of that Moment the Event was triggered
		_key getCurrentKeyCodes();
		
		//! Set KeyCode State of that Moment the Event was triggered
		void setCurrentKeyCodes( _key code );
		
		//! get Rects to be repainted
		_area getDamagedRects();
		
		//! Set Rects to be repainted
		void setDamagedRects( _area rects );
		
		//! Check if this event was auto-generated to refresh every gadget until the DOM-Tree HEAD
		bool isBubblePrevented();
		
		//! Set if this event was auto-generated
		void preventBubble( bool bR = true );
		
		//! Check if the Event crops the area to be painted to
		bool hasClippingRects();
		
		void dump();
};

#endif