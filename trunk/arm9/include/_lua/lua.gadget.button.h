// Check if already included
#ifndef _WIN_L_BUTTON_
#define _WIN_L_BUTTON_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.button.h"

/**
 * Proxy Classes
 */
class _lua_button : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_button( lua_State* L );
		
		//! C-Ctor
		_lua_button( _button* b ) : _lua_gadget( b , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_button>::FunctionType methods[];
		static Lunar<_lua_button>::PropertyType properties[];
		using baseclasses = Lunar<_lua_button>::BaseClassType<_lua_gadget>;
};

#endif