// Check if already included
#ifndef _WIN_L_CLOCKGADGET_
#define _WIN_L_CLOCKGADGET_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.clock.h"

/**
 * Proxy Classes
 */
class _lua_clockgadget : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_clockgadget( lua_State* L );
		
		//! C-Ctor
		_lua_clockgadget( _clockGadget* c ) : _lua_gadget( c , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_clockgadget>::FunctionType methods[];
		static Lunar<_lua_clockgadget>::PropertyType properties[];
		using baseclasses = Lunar<_lua_clockgadget>::BaseClassType<_lua_gadget>;
};

#endif