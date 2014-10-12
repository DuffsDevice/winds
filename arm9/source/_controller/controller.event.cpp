#include <_controller/controller.event.h>
#include <_type/type.gadget.h>

void _eventController::removeEventsOf( _gadget* g )
{
	_eventList& buffer = eventBuffer[curEventBuffer];
	buffer.erase(
		remove_if( buffer.begin() , buffer.end()
			, [g]( const _pair<_event,_eventCallType>& event )->bool{ return event.first.getDestination() == g; }
		)
		, buffer.end()
	);
	return;
	_eventList& buffer2 = eventBuffer[!curEventBuffer];
	
	// Remove from the currently iterated list
	if( iteratingEvents )
		for( _pair<_event,_eventCallType>& event : buffer2 ){
			if( event.first.getDestination() == g )
				event.first.setDestination( nullptr );
		}
	else
		buffer2.erase(
			remove_if( buffer2.begin() , buffer2.end()
				, [g]( const _pair<_event,_eventCallType>& event )->bool{ return event.first.getDestination() == g; }
			)
			, buffer2.end()
		);
}

bool _eventController::init(){
	curEventBuffer = 0;
	iteratingEvents = false;
	return true;
}

void _eventController::end(){
	curEventBuffer = 0;
	eventBuffer[0].clear();
	eventBuffer[1].clear();
}

void _eventController::optimizeEvents()
{
	// Shortcut
	_eventList& events = _eventController::eventBuffer[!_eventController::curEventBuffer];
	
	sort( events.begin() , events.end() ,
		[]( _pair<_event,_eventCallType> e1 , _pair<_event,_eventCallType> e2)->bool
		{
			_eventType eT1 = e1.first.getType();
			_eventType eT2 = e2.first.getType();
			
			if( eT1 == eT2 ){
				if( !isEventTypeMergeable( eT1 ) )
					return false;
				return e1.first.getDestination() < e2.first.getDestination();
			}
			else if( !isEventTypeMergeable( eT1 ) && !isEventTypeMergeable( eT2 ) )
				return false;
			
			return e1.first.getType() < e2.first.getType();
		}
	);
	
	events.erase(
		unique( events.begin() , events.end() , []( _pair<_event,_eventCallType>& e1 , _pair<_event,_eventCallType>& e2 )->bool{ return e1.second == e2.second && e1.first.mergeWith( e2.first ); } )
		, events.end()
	);
}


void _eventController::frame()
{
	// Do not throw any Events until we finished iterating through Events!!!!
	// -> This was a big Problem - Hours of finding that out!
	curEventBuffer = !curEventBuffer;
	iteratingEvents = true;
	
	// Optimize out unnecessary events
	_eventController::optimizeEvents();
	
	for( _pair<_event,_eventCallType>& data : _eventController::eventBuffer[!_eventController::curEventBuffer] )
	{
		// Temp...
		_event& event = data.first;
		_gadget* gadget = event.getDestination();
		
		// Make the Gadget ( if one is specified ) react on the event
		if( gadget != nullptr )
		{
			//printf("Calling %s",gadgetType2string[gadget->getType()]);
			
			// Choose the right call-method
			switch( data.second )
			{
				case _eventCallType::normal:
					gadget->handleEvent( move(event) );
					break;
				case _eventCallType::normalNoDef:
					gadget->handleEvent( move(event) , true );
					break;
				case _eventCallType::user:
					gadget->handleEventUser( move(event) );
					break;
				case _eventCallType::internal:
					gadget->handleEventInternal( move(event) );
					break;
				case _eventCallType::def:
					gadget->handleEventDefault( move(event) );
					break;
				default:
					break;
			}
			//printf(" ;D\n");
		}
	}
	
	_eventController::eventBuffer[!curEventBuffer].clear();
	iteratingEvents = false;
}

_eventList	_eventController::eventBuffer[2];
int			_eventController::curEventBuffer;
bool		_eventController::iteratingEvents;