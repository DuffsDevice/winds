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

extern _fromStr<_eventCallType>	string2eventCallType;
extern _toStr<_eventCallType>	eventCallType2string;

#endif