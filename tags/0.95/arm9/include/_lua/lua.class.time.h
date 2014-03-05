// Check if already included
#ifndef _WIN_L_TIME_
#define _WIN_L_TIME_

#include "_lua/lua.lunar.h"
#include "_type/type.time.h"

/**
 * Proxy Classes
 */
class _lua_time : public _time
{
	public:
		
		//! Lua-Ctor
		_lua_time( lua_State* L );
		
		//! C-Ctor
		_lua_time( _time t ) : _time( t ) {}
		
		//! now
		static int now( lua_State* L );
		
		//! date
		static int date( lua_State* L );
		
		//! time
		static int time( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_time>::FunctionType	methods[];
		static Lunar<_lua_time>::PropertyType	properties[];
		static Lunar<_lua_time>::StaticType		staticmethods[];
	
};

#endif