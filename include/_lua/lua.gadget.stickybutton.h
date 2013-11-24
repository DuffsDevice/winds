// Check if already included
#ifndef _WIN_L_STICKYBUTTON_
#define _WIN_L_STICKYBUTTON_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.stickybutton.h"

/**
 * Proxy Classes
 */
class _lua_stickybutton : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_stickybutton( lua_State* L );
		
		//! C-Ctor
		_lua_stickybutton( _stickybutton* c ) : _lua_gadget( c , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_stickybutton>::FunctionType methods[];
		static Lunar<_lua_stickybutton>::PropertyType properties[];
	
};

#endif