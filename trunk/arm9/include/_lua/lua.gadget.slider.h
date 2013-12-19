// Check if already included
#ifndef _WIN_L_SLIDER_
#define _WIN_L_SLIDER_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.slider.h"

/**
 * Proxy Classes
 */
class _lua_slider : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_slider( lua_State* L );
		
		//! C-Ctor
		_lua_slider( _slider* c ) : _lua_gadget( c , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_slider>::FunctionType methods[];
		static Lunar<_lua_slider>::PropertyType properties[];
	
};

#endif