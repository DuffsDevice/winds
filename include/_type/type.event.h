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
	handled_no_delete = 2,
	use_default = 3,
	not_handled = 0, // If returned on DragStart -> dragging will be prevented
	not_handled_no_delete = -1
};

/**
 * Specifies the Type of an Event
**/
enum _gadgetEventType{
	none	 			= 0,
	mouseClick 			= 1,
	mouseDown 			= 1<<1,
	mouseUp 			= 1<<2,
	mouseDoubleClick 	= 1<<3,
	keyDown 			= 1<<4,
	keyUp 				= 1<<5,
	keyClick 			= 1<<6,
	dragging 			= 1<<7,
	dragStart 			= 1<<8,
	dragStop 			= 1<<9,
	refresh 			= 1<<10,
	individual 			= 1<<11,
	change				= 1<<13,
	focus				= 1<<14,
	blur				= 1<<15,
	all					= (1<<16) - 1
};

// Predefines
extern map<_gadgetEventType,string> eventType2string;
extern map<_gadgetEventReturnType,string> eventReturnType2string;
extern map<string,_gadgetEventType> string2eventType;
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
		
		//! Default Constructors
		_gadgetEvent( _gadget* src ,  _gadgetEventType type , _gadgetEventArgs args = _gadgetEventArgs() );
		_gadgetEvent( _gadgetEventType type = refresh , _gadgetEventArgs args = _gadgetEventArgs() );
		
		// Set type to refresh & set the refreshRect
		_gadgetEvent( _gadget* src , _gadgetEventType type , _area damagedRects = _area() , _gadgetEventArgs args = _gadgetEventArgs() );
		
		// Auto set the Destination
		_gadgetEvent( _gadget* src , _gadgetEventType type , _gadget* dest , _gadgetEventArgs args = _gadgetEventArgs() );
		
		// Manually set Event Type
		void setType( _gadgetEventType type );
		
		// Get EventType
		_gadgetEventType getType();
		
		// Get EventType as a String
		const string getTypeString();
		
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