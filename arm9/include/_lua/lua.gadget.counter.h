// Check if already included
#ifndef _WIN_L_COUNTER_
#define _WIN_L_COUNTER_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.counter.h"

/**
 * Proxy Classes
 */
class _lua_counter : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_counter( lua_State* L );
		
		//! C-Ctor
		_lua_counter( _counter* c ) : _lua_gadget( c , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_counter>::FunctionType methods[];
		static Lunar<_lua_counter>::PropertyType properties[];
		using baseclasses = Lunar<_lua_counter>::BaseClassType<_lua_gadget>;
};

#endif