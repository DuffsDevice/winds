#ifndef _WIN_C_EVENT_
#define _WIN_C_EVENT_

#include <_type/type.h>
#include <_type/type.controller.h>
#include <_type/type.event.h>
#include <_type/type.event.calltype.h>

using _eventList = _vector<_pair<_event,_eventCallType>>;

class _eventController : public _controller
{
	private:
		
		static bool			iteratingEvents;
		static int			curEventBuffer;
		static _eventList	eventBuffer[2];
		
		static void optimizeEvents();
		
	public:
		
		//! Routine to initialize the event buffers
		static bool init();
		
		//! Routine to process all buffered events
		static void frame();
		
		//! Shut down the event controller
		static void end();
		
		//! Push a new event to the currently active buffer
		static void pushEvent( _event&& event , _eventCallType callType ){
			eventBuffer[curEventBuffer].emplace_back( move(event) , callType );
		}
		
		//! Makes sure that no events concerning a _gadget are on the buffer anymore (for _gadget::~_gadget)
		static void removeEventsOf( _gadget* g );
};

#endif