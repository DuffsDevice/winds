// Check if already included
#ifndef _WIN_EVENT_
#define _WIN_EVENT_

#include "type.h"
#include "type.rect.h"
#include "type.eventArgs.h"
#include <map>
using namespace std;

/**
 * Specifies if the Eevent was Handled or not
**/
enum _gadgetEventReturnType{
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
		_gadgetEventArgs args;
		
		_gadget* that;
		
		//! Set the Gadget that received the event (Only Used in gadget::handleEvent)
		void setGadget( _gadget* that );
	
	public:
		
		//! Default Constructor
		_gadgetEvent( _gadget* src = nullptr , _gadgetEventType type = "none" , _gadgetEventArgs args = _gadgetEventArgs() );
		
		//! Common Constructor
		_gadgetEvent( _gadgetEventType type , _gadgetEventArgs args = _gadgetEventArgs() );
		
		//! Specific Events:
		static _gadgetEvent refreshEvent( _gadget* src ,  _area damagedRects = _area() , _gadgetEventArgs args = _gadgetEventArgs() );
		static _gadgetEvent dialogClose( _gadget* src , _s32 intVal , string strVal , _gadgetEventArgs args = _gadgetEventArgs() );
		
		// Manually set Event Type
		void setType( _gadgetEventType type );
		
		// Get EventType
		_gadgetEventType getType();
		
		// Manually set Event Args
		void setArgs( _gadgetEventArgs args );
		
		// Get EventArgs
		_gadgetEventArgs& getArgs();
		
		//! Get the Current Gadget the Handler was called
		_gadget* getGadget();
		
		// Dump whole Class
		void dump();
};

#include "_type/type.gadget.h"

#endif