// Check if already included
#ifndef _WIN_EVENT_
#define _WIN_EVENT_

#include "type.h"
#include "type.rect.h"

/**
 * Specifies how the event was handled
**/
enum _gadgetEventReturnType : _u8
{
	handled = 1,
	use_default = 2,
	use_normal = 3,
	not_handled = 0, // If returned on dragStart -> dragging will be prevented
};

/**
 * Specifies the Type of an Event
**/
typedef string _gadgetEventType;

// Predefines
extern map<_gadgetEventReturnType,string> eventReturnType2string;
extern map<string,_gadgetEventReturnType> string2eventReturnType;

class _gadget;

class _gadgetEvent
{
	private:
		
		friend class _gadget;
		
		_gadgetEventType type;
		
		_gadget* that;
		
		//! Set the Gadget that received the event (Only Used in gadget::handleEvent)
		void setGadget( _gadget* that ){ this->that = that; }
		
		/**
		 * Parameters
		**/
		void* 	src;			//! Triggering Gadget not defined sometimes
		void* 	dest;			//! Gadget to receive the Event
		_coord 	posX;			//! X-Position of the Mouse when the Event was triggered
		_coord 	posY;			//! Y-Position of the Mouse when the Event was triggered
		_coord 	effectiveX;		//! X-Position of the Stylus on the Screen when the Event was triggered
		_coord 	effectiveY;		//! Y-Position of the Stylus on the Screen when the Event was triggered
		_coord 	deltaX;			//! Delta-X, used at dragging-events
		_coord 	deltaY;			//! Delta-Y, used at dragging-events
		_key 	keyCode;		//! KeyCode of the Button that triggered the Event
		_key 	currentKeyCodes;//! Keycode-State of that Moment the Event was triggered
		_u32	heldTime; 		//! Time the Button was Pressed (only for: keyUp, keyClick, mouseUp, mouseClick )
		_area	damagedRects;	//! If (isBubbleRefresh) is true then this Attribute will specify the Area, that is invalid/damaged and has to be repainted
		bool	bubble;	 		//! If 'true', all events needed for informing the parents are already thrown, no need to throw the again
	
	public:
		
		//! Default Constructor
		_gadgetEvent( _gadget* src = nullptr , _gadgetEventType type = "none" );
		
		//! Common Constructor
		_gadgetEvent( _gadgetEventType type );
		
		//! Specific Events:
		static _gadgetEvent refreshEvent( _gadget* src ,  _area damagedRects = _area() );
		static _gadgetEvent dialogClose( _gadget* src , _s32 intVal , string strVal );
		
		//! Manually set Event Type
		_gadgetEvent& setType( _gadgetEventType type ){ this->type = type; return *this; }
		
		//! Get the Type of the Event
		_gadgetEventType getType(){	return this->type; }

		//! Get the Current handling Gadget
		_gadget* getGadget(){ return this->that; }
		
		//! Get the (current) Gadget the Handler was called on
		template<typename T>
		T* getGadget(){ 
			#pragma GCC diagnostic ignored "-Wunused-value"
			T::_gadget::defaultEventHandlers; // Just Test if the supplied param is a subclass of _gadget!!!!!
			#pragma GCC diagnostic warning "-Wunused-value"
			return static_cast<T*>( this->that );
		}
		
		/**
		 * Parameters
		**/		
		void resetParams( void* dest = nullptr );//!............................<= Reset All Arguments
		
		//! Setters...
		_gadgetEvent& setDestination( void* newVal ){ this->dest = newVal; return *this; }//!..........<= Set the Destination
		_gadgetEvent& setSource( void* newVal ){ this->src = newVal; return *this; }//!................<= Set the Source
		_gadgetEvent& setPosX( _coord val ){ this->posX = val; return *this; }//!......................<= Set Triggering Point X
		_gadgetEvent& setPosY( _coord val ){ this->posY = val; return *this; }//!......................<= Set Triggering Point Y
		_gadgetEvent& setEffectivePosX( _coord val ){ this->effectiveX = val; return *this; }//!.......<= Set Triggering Point X which results in the position on the screen that the user effectively touched
		_gadgetEvent& setEffectivePosY( _coord val ){ this->effectiveY = val; return *this; }//!.......<= Set Triggering Point Y which results in the position on the screen that the user effectively touched
		_gadgetEvent& setDeltaX( _coord val ){ this->deltaX = val; return *this; }//!..................<= Set Triggering Point X
		_gadgetEvent& setDeltaY( _coord val ){ this->deltaY = val; return *this; }//!..................<= Set Triggering Point Y
		_gadgetEvent& setKeyCode( _key code ){ this->keyCode = code; return *this; }//!................<= Set triggering KeyCode
		_gadgetEvent& setHeldTime( _u32 heldTime ){ this->heldTime = heldTime; return *this; }//!......<= Set Held Time of the key that triggered the Event
		_gadgetEvent& setCurrentKeyCodes( _key code ){ this->currentKeyCodes = code; return *this; }//!<= Set KeyCode State of that Moment the Event was triggered
		_gadgetEvent& preventBubble( bool bR = true ){ this->bubble = bR; return *this; }//!...........<= Set if this event was auto-generated
		_gadgetEvent& setDamagedRects( _area rects ){ this->damagedRects = rects; return *this; }//!...<= Set Rects to be repainted
		
		//! Getters
		void* getDestination(){ return this->dest; }//!.........................<= Get Destination Gadget
		void* getSource(){ return this->src; }//!...............................<= Get Trigger Gadget
		_coord getPosX(){ return this->posX; }//!...............................<= Get Triggering Point X
		_coord getPosY(){ return this->posY; }//!...............................<= Get Triggering Point Y
		_coord getEffectivePosX(){ return this->effectiveX; }//!................<= Get effective Triggering Point X
		_coord getEffectivePosY(){ return this->effectiveY; }//!................<= Get effective Triggering Point Y
		_coord getDeltaX(){ return this->deltaX; }//!...........................<= Get Delta-X
		_coord getDeltaY(){ return this->deltaY; }//!...........................<= Get Delta-Y
		_key getKeyCode(){ return this->keyCode; }//!...........................<= Get triggering KeyCode
		
		_u32 getHeldTime(){ return this->heldTime; }//!.........................<= Get Held Time of the key that triggered the Event
		_key getCurrentKeyCodes(){ return this->currentKeyCodes; }//!...........<= Get KeyCode State of that Moment the Event was triggered
		_area getDamagedRects(){ return this->damagedRects; }//!................<= get Rects to be repainted
		bool isBubblePrevented(){ return this->bubble; }//!.....................<= Check if this event was auto-generated to refresh every gadget until the DOM-Tree HEAD
		bool hasClippingRects(){ return !this->damagedRects.empty(); } //!......<= Check if the Event crops the area to be painted on
};

#include "_type/type.gadget.h"

#endif