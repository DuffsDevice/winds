#ifndef _WIN_T_TIME
#define _WIN_T_TIME

#include "_type/type.h"

struct _time
{
	_u8 month : 4;
	_u8 day : 5;
	_u8 hour : 5;
	_u8 minute : 6;
	_u8 second : 6;
	_u8 weekday : 3;
	_u16 year : 13;
	
	//! Get a time struct of >>now<<
	static _time now();
	
	//! Ctor
	_time()
	{
		*this = now();
	}
	
	//! Ctor with UNIX Time
	_time( _u32 );
	
	//! Convert the _time structure to UNIX time
	operator _u32();
	
	//! Convert the _time to string
	operator string();
	
	//! Convert the _time to string using a specific format
	//! @see http://www.cplusplus.com/reference/ctime/strftime/#parameters
	string toString( string format );
};

#endif