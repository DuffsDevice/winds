// Check if already included
#ifndef _WIN_L_SCROLLBAR_
#define _WIN_L_SCROLLBAR_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.scrollBar.h"

/**
 * Proxy Classes
 */
class _lua_scrollbar : public _lua_gadget
{	
	public:
		
		//! Lua-Ctor
		_lua_scrollbar( lua_State* L );
		
		//! C-Ctor
		_lua_scrollbar( _scrollBar* s ) : _lua_gadget( s , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_scrollbar>::FunctionType methods[];
		static Lunar<_lua_scrollbar>::PropertyType properties[];
	
};

#endif