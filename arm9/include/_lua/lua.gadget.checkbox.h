// Check if already included
#ifndef _WIN_L_checkBox_
#define _WIN_L_checkBox_

#include <_lua/lua.lunar.h>
#include <_lua/lua.class.gadget.h>
#include <_gadget/gadget.checkbox.h>

/**
 * Proxy Classes
 */
class _lua_checkbox : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_checkbox( lua_State* L );
		
		//! C-Ctor
		_lua_checkbox( _checkBox* c ) : _lua_gadget( c , false )
		{ }
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_checkbox>::FunctionType methods[];
		static Lunar<_lua_checkbox>::PropertyType properties[];
		using baseclasses = Lunar<_lua_checkbox>::BaseClassType<_lua_gadget>;
};

#endif