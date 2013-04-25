// Check if already included
#ifndef _WIN_EVENT_
#define _WIN_EVENT_

#include "type.h"
#include "type.rect.h"

/**
 * Specifies how the event was handled
 */
enum _callbackReturn : _s8
{
	not_handled = 0, // If returned on dragStart -> dragging will be prevented
	handled = 1,
	use_default = 2,
	prevent_default = -1,
};

/**
 * Specifies the Type of an Event
 */
enum _eventType : _u8
{
	_none_,
	_internal_,
	refresh,
	mouseClick,
	mouseDoubleClick,
	mouseDown,
	mouseUp,
	mouseRepeat, // Fired if the stylus is kept held down
	keyDown,
	keyUp,
	keyClick,
	keyRepeat, // Fired if a key is kept held down
	dragStart,
	dragStop,
	dragging,
	onResize,
	onAction,
	onChange,
	onBlur,
	onFocus,
	onOpen,
	onClose,
	onMouseEnter,
	onMouseLeave,
	onMaximize,
	onUnMaximize,
	onMinimize,
	onRestore,
	onScroll,
	onStyleSet
};


//typedef string _eventType;

// Predefines
extern _map<_callbackReturn,string> callbackReturn2string;
extern _map<string,_callbackReturn> string2callbackReturn;

extern _map<_eventType,string> eventType2string;
extern _map<string,_eventType> string2eventType;

class _gadget;

class _event
{
	friend class _gadget;
	
	private:
		
		//! Type of event
		_eventType	type;
		
		//! Set the Gadget that received the event (Only Used in gadget::handleEvent)
		void setGadget( _gadget* d ) const { this->gadget = d; }
		
		/**
		 * Parameters
		 */
		mutable _gadget* 	gadget;			//! Gadget to receive the Event
		_coord 				posX;			//! X-Position of the Mouse when the Event was triggered
		_coord 				posY;			//! Y-Position of the Mouse when the Event was triggered
		_coord 				effectiveX;		//! X-Position of the Stylus on the Screen when the Event was triggered
		_coord 				effectiveY;		//! Y-Position of the Stylus on the Screen when the Event was triggered
		_coord 				deltaX;			//! Delta-X, used at dragging-events
		_coord 				deltaY;			//! Delta-Y, used at dragging-events
		_key 				keyCode;		//! KeyCode of the Button that triggered the Event
		_key 				currentKeyCodes;//! Keycode-State of that Moment the Event was triggered
		_u32				heldTime; 		//! Time the Button was Pressed (only for: keyUp, keyClick, mouseUp, mouseClick )
		mutable _area		damagedRects;	//! If (isBubbleRefresh) is true then this Attribute will specify the Area, that is invalid/damaged and has to be repainted
	
	public:
		
		//! Default Constructor
		_event()
		{
		}
		
		_event( _eventType type ) :
			type( type ) 
			, posX( 0 ) , posY( 0 )
			, effectiveX( 0 ) , effectiveY( 0 )
			, deltaX( 0 ) , deltaY( 0 )
			, keyCode( 0 ) , currentKeyCodes( 0 )
			, heldTime( 0 )
		{ }
		
		//! Specific Events:
		static _event refreshEvent( _area damagedRects ){
			return _event( refresh ).setDamagedRects( damagedRects );
		}
		static _event refreshEvent(){
			return _event( refresh );
		}
		
		//! Manually set Event Type
		_event& setType( _eventType type ){ this->type = type; return *this; }
		
		//! Get the Type of the Event
		_eventType getType() const { return this->type; }

		//! Get the Current handling Gadget
		_gadget* getGadget() const { return this->gadget; }
		
		//! Get the (current) Gadget the Handler was called on
		template<typename T>
		T* getGadget() const { 
			// Just Test if the supplied param is a subclass of _gadget!!!!!
			subclass_of<T,_gadget>();
			return static_cast<T*>( this->gadget );
		}
		
		/**
		 * Parameters
		 */		
		void resetParams( _gadget* dest = nullptr );//!............................<= Reset All Arguments
		
		//! Setters...
		_event& setDestination( _gadget* newVal ){ this->gadget = newVal; return *this; }//!........<= Set the Destination
		_event& setPosX( _coord val ){ this->posX = val; return *this; }//!.........................<= Set Triggering Point X
		_event& setPosY( _coord val ){ this->posY = val; return *this; }//!.........................<= Set Triggering Point Y
		_event& setEffectivePosX( _coord val ){ this->effectiveX = val; return *this; }//!..........<= Set Triggering Point X which results in the position on the screen that the user effectively touched
		_event& setEffectivePosY( _coord val ){ this->effectiveY = val; return *this; }//!..........<= Set Triggering Point Y which results in the position on the screen that the user effectively touched
		_event& setDeltaX( _coord val ){ this->deltaX = val; return *this; }//!.....................<= Set Triggering Point X
		_event& setDeltaY( _coord val ){ this->deltaY = val; return *this; }//!.....................<= Set Triggering Point Y
		_event& setKeyCode( _key code ){ this->keyCode = code; return *this; }//!...................<= Set triggering KeyCode
		_event& setHeldTime( _u16 heldTime ){ this->heldTime = heldTime; return *this; }//!.........<= Set Held Time of the key that triggered the Event
		_event& setCurrentKeys( _key code ){ this->currentKeyCodes = code; return *this; }//!.......<= Set KeyCode State of that Moment the Event was triggered
		const _event& setDamagedRects( _area rects ) const { this->damagedRects = rects; return *this; }//!<= Set Rects to be repainted
		
		//! Getters
		_gadget* getDestination() const { return this->gadget; }//!...............<= Get Destination Gadget
		_coord getPosX() const { return this->posX; }//!..........................<= Get Triggering Point X
		_coord getPosY() const { return this->posY; }//!..........................<= Get Triggering Point Y
		_coord getEffectivePosX() const { return this->effectiveX; }//!...........<= Get effective Triggering Point X
		_coord getEffectivePosY() const { return this->effectiveY; }//!...........<= Get effective Triggering Point Y
		_coord getDeltaX() const { return this->deltaX; }//!......................<= Get Delta-X
		_coord getDeltaY() const { return this->deltaY; }//!......................<= Get Delta-Y
		_key getKeyCode() const { return this->keyCode; }//!......................<= Get triggering KeyCode
		_u16 getHeldTime() const { return this->heldTime; }//!....................<= Get Held Time of the key that triggered the Event
		_key getCurrentKeys() const { return this->currentKeyCodes; }//!..........<= Get KeyCode State of that Moment the Event was triggered
		_area getDamagedRectsCopy() const { return this->damagedRects; }//!.......<= Get Rects to be repainted (a modifyable copy of them)
		_area& getDamagedRectsConst() const { return this->damagedRects; }//!.....<= Get Rects to be repainted (a modifyable copy of them)
		_area& getDamagedRects(){ return this->damagedRects; }//!.................<= Get Rects to be repainted
		bool hasClippingRects() const { return !this->damagedRects.empty(); } //!.<= Check if the Event crops the area to be painted on
};

#endif