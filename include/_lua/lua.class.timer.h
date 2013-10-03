// Check if already included
#ifndef _WIN_L_TIMER_
#define _WIN_L_TIMER_

#include "_lua/lunar.h"
#include "_type/type.timer.h"

/**
 * Proxy Classes
 */
class _lua_timer : public _timer
{
	public:
		
		//! Constructor
		_lua_timer( lua_State* L );
		
		//! setCallback
		int setCallback( lua_State* L );
		
		//! deleteCallback
		int deleteCallback( lua_State* L );
		
		//! start
		int start( lua_State* L );
		
		//! stop
		int stop( lua_State* L );
		
		//! getRepeating
		int getRepeating( lua_State* L );
		
		//! setRepeating
		int setRepeating( lua_State* L );
		
		//! getDuration
		int getDuration( lua_State* L );
		
		//! setDuration
		int setDuration( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_timer>::FunctionType methods[];
		static Lunar<_lua_timer>::PropertyType properties[];
	
};

#endif