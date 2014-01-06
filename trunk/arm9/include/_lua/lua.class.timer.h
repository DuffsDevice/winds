// Check if already included
#ifndef _WIN_L_TIMER_
#define _WIN_L_TIMER_

#include "_lua/lua.lunar.h"
#include "_type/type.timer.h"

/**
 * Proxy Classes
 */
class _lua_timer : public _timer
{
	public:
		
		//! Constructor
		_lua_timer( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_timer>::FunctionType methods[];
		static Lunar<_lua_timer>::PropertyType properties[];
	
};

#endif