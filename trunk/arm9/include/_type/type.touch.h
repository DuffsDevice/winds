#ifndef _WIN_T_TOUCH_
#define _WIN_T_TOUCH_

#include <_type/type.h>

#define u16 _u16
#include <nds/touch.h>
#undef u16

//! Holds useful information about the touch of the stylus
struct _touch
{
	_coord x;
	_coord y;
	
	_touch& operator=( touchPosition t )
	{
		x = t.px;
		y = t.py;
		return *this;
	}
	
	_touch( touchPosition t ){ *this = t; }
	
	_touch() :
		x( 0 )
		, y ( 0 )
	{ }
};

#endif