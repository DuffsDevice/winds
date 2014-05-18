// Check if already included
#ifndef _WIN_EVENT_
#define _WIN_EVENT_

#include "_type/type.h"
#include "_type/type.rect.h"
#include "_type/type.flexptr.h"
#include "_type/type.dependency.h"
#include "_type/type.key.h"

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
enum _eventType : _u8
{
	_none_,
	onDraw,
	onMouseClick,
	onMouseDblClick,
	onMouseDown,
	onMouseUp,
	onMouseRepeat, // Fired if the stylus is kept held down
	onKeyDown,
	onKeyUp,
	onKeyClick,
	onKeyRepeat, // Fired if a key is kept held down
	onMouseRightClick,
	onDragStart,
	onDragStop,
	onDragging,
	
	onUpdate,
	onEdit,
	onOpen,
	onClose,
	onMouseEnter,
	onMouseLeave,
	onMaximize,
	onUnMaximize,
	onMinimize,
	onRestore,
	onScroll,
	
	//
	// Events from various perspectives
	//
	onResize,
	onMove,
	onRestyle,
	onShow,
	onHide,
	onFocus,
	onBlur,
	onAdd, // Not Used
	onDelete,
	onSelect,
	onDeselect,
	
	// From the perspective of a child
	onParentResize,
	onParentMove,
	onParentRestyle,
	onParentShow,
	onParentHide,
	onParentFocus,
	onParentBlur,
	onParentAdd,
	onParentRemove,
	onParentSelect,
	onParentDeselect,
	
	// From the perspective of a parent
	onChildResize,
	onChildMove,
	onChildRestyle,
	onChildShow,
	onChildHide,
	onChildFocus,
	onChildBlur,
	onChildAdd,
	onChildRemove,
	onChildSelect,
	onChildDeselect,
	
	// From the perspective of a subcedent child
	onPreResize,
	onPreMove,
	onPreRestyle,
	onPreShow,
	onPreHide,
	onPreFocus,
	onPreBlur,
	onPreAdd,
	onPreRemove,
	onPreSelect,
	onPreDeselect,
	
	// From the perspective of a precedent child
	onPostResize,
	onPostMove,
	onPostRestyle,
	onPostShow,
	onPostHide,
	onPostFocus,
	onPostBlur,
	onPostAdd,
	onPostRemove,
	onPostSelect,
	onPostDeselect
};

//! Converts a dependency EventType to a more specific type
unused static inline constexpr _eventType depType2parentType( _eventType et ){ return _eventType( et + ( onParentResize - onResize ) ); }
unused static inline constexpr _eventType depType2childType( _eventType et ){ return _eventType( et + ( onChildResize - onResize ) ); }
unused static inline constexpr _eventType depType2preType( _eventType et ){ return _eventType( et + ( onPreResize - onResize ) ); }
unused static inline constexpr _eventType depType2postType( _eventType et ){ return _eventType( et + ( onPostResize - onResize ) ); }
unused static inline constexpr bool isDepType( _eventType et ){ return et >= onParentResize && et <= onPostDeselect; }

//! Checks if an eventType is a user-Event-type
unused static inline bool constexpr isEventTypeMergeable( _eventType et ){
	return et > onMouseRightClick;
}

//! Checks if an eventType is a user-Event-type
unused static inline bool constexpr isUserET( _eventType et ){
	return et & ( 1 << 7 );
}

//! Converts an internal event-type to an user-eventType
unused static inline constexpr _eventType eventType2userET( _eventType et ){
	return isUserET( et ) ? et :  _eventType( et | ( 1 << 7 ) );
}

//! Converts an user-eventType to an internal event-type
unused static inline constexpr _eventType userET2eventType( _eventType et ){
	return isUserET( et ) ? _eventType( _u16(et) ^ ( 1 << 7 ) ) : et;
}

//! Uncomment if a string should indicate
//! the type of an event instead of an enum
//typedef string _eventType;

// Predefines
class _gadget;
extern _toStr<_callbackReturn>		callbackReturn2string;
extern _fromStr<_callbackReturn>	string2callbackReturn;
extern _toStr<_eventType>			eventType2string;
extern _fromStr<_eventType>			string2eventType;

struct _dependencyParam
{
	_gadget*	gadget;				//! Gadget-parameter
	_u8			flag;				//! Boolean-parameter
	
	_dependencyParam( _u8 val = 0 ) : 
		gadget( nullptr )
		, flag( val )
	{}
	
	_dependencyParam& setSource( _gadget* g ){ gadget = g; return *this; }
};

class _event
{
	friend class _gadget;
	
	private:
		
		//! Type of event
		_eventType			type;
		
		/**
		 * Parameters
		 */
		mutable _gadget*		gadget;			//! Gadget to receive the Event
		_int 					posX;			//! X-Position of the Mouse when the Event was triggered
		_int 					posY;			//! Y-Position of the Mouse when the Event was triggered
		_int 					effectiveX;		//! X-Position of the Stylus on the Screen when the Event was triggered
		_int 					effectiveY;		//! Y-Position of the Stylus on the Screen when the Event was triggered
		_hardwareKeyPattern		currentKeyCodes;//! Keycode-State of that Moment the Event was triggered
		_flexPtr<_area>			damagedRects;	//! this Attribute will specify the Area, that is invalid/damaged and has to be repainted
		_int 					deltaX;			//! Delta-X, used at dragging-events
		_int 					deltaY;			//! Delta-Y, used at dragging-events
		union{
			//! Only used at: keyUp, keyClick
			struct{
				_key			keyCode;		//! KeyCode of the Button that triggered the Event
				_u32			heldTime;		//! Duration the Button/Stylus was pressed
				_u32			pressure;		//! Indicates how strong the stylus was pressed on the screen
			};
			
			//! Only used at onDependencyChange
			_dependencyParam	depParam;
		};
		
	
	public:
		
		//! Constructor
		_event( _eventType type = _none_ ) :
			type( type )			, gadget( nullptr )
			, posX( 0 )				, posY( 0 )
			, effectiveX( 0 )		, effectiveY( 0 )
			, currentKeyCodes( 0 )	, deltaX( 0 )
			, deltaY( 0 )			, keyCode( _key::none )
			, heldTime( 0 )			, pressure( 0 )
		{ }
		
		//! Shortcut to generate specific Events:
		static _event drawEvent( const _area& damagedRects ){ return drawEvent( _area( damagedRects ) ); }
		static _event drawEvent( _area&& damagedRects ){
			_event evt;
			evt.type = onDraw;
			evt.damagedRects = move(damagedRects);
			return move(evt);
		}
		
		static _event dependencyEvent( _eventType type , _dependencyParam param ){
			_event evt;
			evt.type = type;
			evt.depParam = param;
			return (evt);
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
			unused typedef typename T::_gadget def; // Tests if the supplied param is a subclass of _gadget
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
		_event& setDestination( _gadget* newVal ){ this->gadget = newVal; return *this; }//!...................<= Set the Destination
		_event& setPressure( _u32 val ){ this->pressure = val; return *this; }//!..............................<= Set Touchscreen Pressure
		_event& setPosX( _int val ){ this->posX = val; return *this; }//!......................................<= Set Triggering Point X
		_event& setPosY( _int val ){ this->posY = val; return *this; }//!......................................<= Set Triggering Point Y
		_event& setEffectivePosX( _int val ){ this->effectiveX = val; return *this; }//!.......................<= Set Triggering Point X which results in the position on the screen that the user effectively touched
		_event& setEffectivePosY( _int val ){ this->effectiveY = val; return *this; }//!.......................<= Set Triggering Point Y which results in the position on the screen that the user effectively touched
		_event& setDeltaX( _int val ){ this->deltaX = val; return *this; }//!..................................<= Set Delta Point X
		_event& setDeltaY( _int val ){ this->deltaY = val; return *this; }//!..................................<= Set Delta Point Y
		_event& setKeyCode( _key code ){ this->keyCode = code; return *this; }//!..............................<= Set triggering KeyCode
		_event& setHeldTime( _u16 heldTime ){ this->heldTime = heldTime; return *this; }//!....................<= Set Held Time of the key that triggered the Event
		_event& setCurrentKeys( _hardwareKeyPattern keys ){ this->currentKeyCodes = keys; return *this; }//!...<= Set KeyCode State of that Moment the Event was triggered
		_event& setDamagedRects( _area rects ){ this->damagedRects = move(rects); return *this; }//!...........<= Set Rects to be repainted
		_event& setFlagParam( bool flag ){ this->depParam.flag = flag; return *this; }//!......................<= Set the dimensions-dependency that changed
		_event& setGadgetParam( _gadget* gadget ){ this->depParam.gadget = gadget; return *this; }//!..........<= Set the dimensions-dependency that changed
		
		//! Getters
		_gadget* getDestination() const { return this->gadget; }//!...............................<= Get Destination Gadget
		_u32 getPressure() const { return this->pressure; }//!....................................<= Get Touchscreen Pressure
		_int getPosX() const { return this->posX; }//!............................................<= Get Triggering Point X
		_int getPosY() const { return this->posY; }//!............................................<= Get Triggering Point Y
		_int getEffectivePosX() const { return this->effectiveX; }//!.............................<= Get effective Triggering Point X
		_int getEffectivePosY() const { return this->effectiveY; }//!.............................<= Get effective Triggering Point Y
		_int getDeltaX() const { return this->deltaX; }//!........................................<= Get Delta-X
		_int getDeltaY() const { return this->deltaY; }//!........................................<= Get Delta-Y
		_key getKeyCode() const { return this->keyCode; }//!......................................<= Get triggering KeyCode
		_u16 getHeldTime() const { return this->heldTime; }//!....................................<= Get Held Time of the key that triggered the Event
		const _hardwareKeyPattern& getCurrentKeys() const { return this->currentKeyCodes; }//!....<= Get KeyCode State of that Moment the Event was triggered
		_area* getDamagedRectsSource(){ return this->damagedRects; }//!...........................<= Get Rects to be repainted (the pointer to the source)
		const _area* getDamagedRectsSource() const { return this->damagedRects; }//!..............<= Get Rects to be repainted (the pointer to the source)
		_area& getDamagedRects(){ return *this->damagedRects; }//!................................<= Get Rects to be repainted (a modifyable copy of them)
		const _area& getDamagedRects() const { return *this->damagedRects; }//!...................<= Get Rects to be repainted (a modifyable copy of them)
		bool hasClippingRects() const {//!........................................................<= Check if the (onDraw-) Event crops the area to be painted on
			return this->damagedRects && !this->damagedRects->isEmpty();
		}
		bool getFlagParam() const { return this->depParam.flag; }//!..............................<= Get the flag param that is brought along events like onParentVisibility events
		_gadget* getGadgetParam() const { return this->depParam.gadget; }//!......................<= Get the _gadget param that is brought along events like onChildSet events
		
		//! Merge this event with another
		bool mergeWith( _event& event );
		
		bool operator==( _eventType type ) const { return this->type == type; }
		bool operator!=( _eventType type ) const { return this->type != type; }
};

#endif