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
	
	static _time now();
	
	_time()
	{
		*this = now();
	}
	
	_time( _u32 );
	
	operator _u32();
	
	operator string();
};

#endif