#include "_type/type.event.calltype.h"

// Convert a string to an eventCallType
_map<string,_eventCallType> string2eventCallType = {
	{ "normal" , _eventCallType::normal },
	{ "normalNoDefault" , _eventCallType::normalNoDef },
	{ "user" , _eventCallType::user },
	{ "internal" , _eventCallType::internal },
	{ "default" , _eventCallType::def }
};


// ...and back
_map<_eventCallType,string> eventCallType2string = {
	{ _eventCallType::normal , "normal" },
	{ _eventCallType::normalNoDef , "normalNoDefault" },
	{ _eventCallType::user , "user" },
	{ _eventCallType::internal , "internal" },
	{ _eventCallType::def , "default" }
};