// Check if already included
#ifndef _WIN_L_EVENT_
#define _WIN_L_EVENT_

#include <_lua/lua.lunar.h>
#include <_type/type.event.h>

class _lua_event : public _event {
	
	public:
		
		//! Ctor...
		_lua_event( _event e );
		
		//! Lua-Ctor...
		_lua_event( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_event>::FunctionType methods[];
		static Lunar<_lua_event>::PropertyType properties[];
};

#endif