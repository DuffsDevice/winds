// Check if already included
#ifndef _WIN_L_WINDOW_
#define _WIN_L_WINDOW_

#include <_lua/lua.lunar.h>
#include <_lua/lua.class.gadget.h>
#include <_gadget/gadget.window.h>

/**
 * Proxy Classes
 */
class _lua_window : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_window( lua_State* L );
		
		//! C-Ctor
		_lua_window( _window* w ) : _lua_gadget( w , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_window>::FunctionType methods[];
		static Lunar<_lua_window>::PropertyType properties[];
		using baseclasses = Lunar<_lua_window>::BaseClassType<_lua_gadget>;
};

#endif