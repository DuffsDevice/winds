// Check if already included
#ifndef _WIN_L_WINDOWBAR_
#define _WIN_L_WINDOWBAR_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.window.bar.h"

/**
 * Proxy Class
 */
class _lua_windowbar : public _lua_gadget
{
	public:
		
		_lua_windowbar( lua_State* L );
		
		_lua_windowbar( _windowBar* c ) : _lua_gadget( c , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_windowbar>::FunctionType methods[];
		static Lunar<_lua_windowbar>::PropertyType properties[];
		using baseclasses = Lunar<_lua_windowbar>::BaseClassType<_lua_gadget>;
};

#endif