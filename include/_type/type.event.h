// Check if already included
#ifndef _WIN_EVENT_
#define _WIN_EVENT_

#include "type.h"
#include "type.rect.h"
#include "type.flexptr.h"

/**
 * Specifies how the event was handled
 */
enum _callbackReturn : _s8
{
	// Returned if anything could not be handled
	not_handled = 0,
	
	// Standard return value (indicates that the event was handled)
	handled = 1,
	
	// Can be returned in userEventHandlers or internalEventHandlers
	// to fall back to the default eventHandler
	use_default = 2,
	
	// Can be retured in userEventHandlers to
	// fall back to the internal eventHandler
	use_internal = 3,
	
	// Used in _gadget; can be returned in dragStart-events to prevent dragging
	prevent_default = -1,
};

/**
 * Specifies the Type of an Event
 */
enum _eventType : _u16
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
	onStyleSet,
	onDelete
};

//! Checks if an eventType is a user-Event-type
unused static inline bool constexpr isUserET( _eventType et ){
	return _u16(et) >> 8;
}

//! Converts an internal event-type to an user-eventType
unused static inline constexpr _eventType eventType2userET( _eventType et ){
	return isUserET( et ) ? et : _eventType( _u16(et) << 8 );
}

//! Converts an user-eventType to an internal event-type
unused static inline constexpr _eventType userET2eventType( _eventType et ){
	return isUserET( et ) ? _eventType( _u16(et) >> 8 ) : et;
}

//! Uncomment if a string should indicate
//! the type of an event instead of an enum
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
		_eventType			type;
		
		/**
		 * Parameters
		 */
		mutable _gadget*	gadget;			//! Gadget to receive the Event
		_coord 				posX;			//! X-Position of the Mouse when the Event was triggered
		_coord 				posY;			//! Y-Position of the Mouse when the Event was triggered
		_coord 				effectiveX;		//! X-Position of the Stylus on the Screen when the Event was triggered
		_coord 				effectiveY;		//! Y-Position of the Stylus on the Screen when the Event was triggered
		_key 				currentKeyCodes;//! Keycode-State of that Moment the Event was triggered
		flex_ptr<_area>		damagedRects;	//! this Attribute will specify the Area, that is invalid/damaged and has to be repainted
		_coord 				deltaX;			//! Delta-X, used at dragging-events
		_coord 				deltaY;			//! Delta-Y, used at dragging-events
		_key 				keyCode;		//! KeyCode of the Button that triggered the Event
		_u32				heldTime; 		//! Time the Button was Pressed (only for: keyUp, keyClick, mouseUp, mouseClick )
	
	public:
		
		//! Default Constructor
		_event() :
			type( _none_ ),
			gadget( nullptr )
		{ }
		
		_event( _eventType type ) :
			type( type ) , gadget( nullptr )
			, posX( 0 ) , posY( 0 )
			, effectiveX( 0 ) , effectiveY( 0 )
			, currentKeyCodes( 0 )
			, deltaX( 0 ) , deltaY( 0 )
			, keyCode( 0 ) , heldTime( 0 )
		{ }
		
		//! Shortcut to generate specific Events:
		static _event refreshEvent( const _area& damagedRects ){ return refreshEvent( _area( damagedRects ) ); }
		static _event refreshEvent( _area&& damagedRects ){
			_event evt{ refresh };
			evt.damagedRects = move(damagedRects);
			return evt;
		}
		
		//! Get the Type of the Event
		_eventType getType() const { return this->type; }
		
		//! Set the Gadget that received the event (Only Used in gadget::handleEvent)
		const _event& setGadget( _gadget* d ) const { this->gadget = d; return *this; }
		
		//! Get the Current handling Gadget
		_gadget* getGadget() const { return this->gadget; }
		
		//! Get the (current) Gadget the Handler was called on
		template<typename T>
		T* getGadget() const { 
			// Just Test if the supplied param is a subclass of _gadget!!!!!
			typedef typename T::_gadget def;
			return static_cast<T*>( this->gadget );
		}
		
		/**
		 * Event-Parameters
		 */
		
		//! Reset All Arguments
		void resetParams( _gadget* dest = nullptr );
		
		//! Setters...
		_event& setType( _eventType type ){//!..........................................................................<= Manually set Event Type
			this->type = type;
			return *this;
		}
		_event& setDestination( _gadget* newVal ){ this->gadget = newVal; return *this; }//!............................<= Set the Destination
		_event& setPosX( _coord val ){ this->posX = val; return *this; }//!.............................................<= Set Triggering Point X
		_event& setPosY( _coord val ){ this->posY = val; return *this; }//!.............................................<= Set Triggering Point Y
		_event& setEffectivePosX( _coord val ){ this->effectiveX = val; return *this; }//!..............................<= Set Triggering Point X which results in the position on the screen that the user effectively touched
		_event& setEffectivePosY( _coord val ){ this->effectiveY = val; return *this; }//!..............................<= Set Triggering Point Y which results in the position on the screen that the user effectively touched
		_event& setDeltaX( _coord val ){ if( this->type != refresh ) this->deltaX = val; return *this; }//!.............<= Set Triggering Point X
		_event& setDeltaY( _coord val ){ if( this->type != refresh ) this->deltaY = val; return *this; }//!.............<= Set Triggering Point Y
		_event& setKeyCode( _key code ){ if( this->type != refresh ) this->keyCode = code; return *this; }//!...........<= Set triggering KeyCode
		_event& setHeldTime( _u16 heldTime ){ if( this->type != refresh ) this->heldTime = heldTime; return *this; }//!.<= Set Held Time of the key that triggered the Event
		_event& setCurrentKeys( _key code ){ this->currentKeyCodes = code; return *this; }//!...........................<= Set KeyCode State of that Moment the Event was triggered
		_event& setDamagedRects( const _area& rects ){ return this->setDamagedRects( _area( rects ) ); }//!.............<= Set Rects to be repainted
		_event& setDamagedRects( _area&& rects ){ this->damagedRects = move(rects); return *this; }//!..................<= Set Rects to be repainted
		
		//! Getters
		_gadget* getDestination() const { return this->gadget; }//!............<= Get Destination Gadget
		_coord getPosX() const { return this->posX; }//!.......................<= Get Triggering Point X
		_coord getPosY() const { return this->posY; }//!.......................<= Get Triggering Point Y
		_coord getEffectivePosX() const { return this->effectiveX; }//!........<= Get effective Triggering Point X
		_coord getEffectivePosY() const { return this->effectiveY; }//!........<= Get effective Triggering Point Y
		_coord getDeltaX() const { return this->deltaX; }//!...................<= Get Delta-X
		_coord getDeltaY() const { return this->deltaY; }//!...................<= Get Delta-Y
		_key getKeyCode() const { return this->keyCode; }//!...................<= Get triggering KeyCode
		_u16 getHeldTime() const { return this->heldTime; }//!.................<= Get Held Time of the key that triggered the Event
		_key getCurrentKeys() const { return this->currentKeyCodes; }//!.......<= Get KeyCode State of that Moment the Event was triggered
		_area* getDamagedRectsSource() const { return this->damagedRects; }//!.<= Get Rects to be repainted (the pointer to the source)
		_area& getDamagedRects(){ return *this->damagedRects; }//!.............<= Get Rects to be repainted (a modifyable copy of them)
		bool hasClippingRects() const {//!.....................................<= Check if the (refresh-) Event crops the area to be painted on
			return this->damagedRects && !this->damagedRects->empty();
		}
		
		//! Merge this event with another
		bool mergeWith( _event& event );
};

#endif