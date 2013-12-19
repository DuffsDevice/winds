// Check if already included
#ifndef _WIN_L_TEXTAREA_
#define _WIN_L_TEXTAREA_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.textarea.h"

/**
 * Proxy Classes
 */
class _lua_textarea : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_textarea( lua_State* L );
		
		//! C-Ctor
		_lua_textarea( _textarea* t ) : _lua_gadget( t , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_textarea>::FunctionType methods[];
		static Lunar<_lua_textarea>::PropertyType properties[];
	
};

#endif