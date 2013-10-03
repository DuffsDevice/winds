#ifndef _WIN_T_EVENTCALLTYPE_
#define _WIN_T_EVENTCALLTYPE_

#include "_type/type.h"

enum class _eventCallType : _u8{
	normal,		// handleEvent( ... , false )
	normalNoDef,// handleEvent( ... , true )
	user,		// handleEventUser( ... )
	internal,	// handleEventInternal( ... )
	def 		// handleEventDefault( ... )
};

extern _map<string,_eventCallType> string2eventCallType;
extern _map<_eventCallType,string> eventCallType2string;

#endif