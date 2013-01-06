// Check if already included
#ifndef _WIN_LUAWINDOW_
#define _WIN_LUAWINDOW_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.window.h"

/**
 * Proxy Classes
**/
class _lua_window : public _lua_gadget , public _lua_interface_input<_window> {	
	
	public:
		
		_lua_window( lua_State* L );
		
		_lua_window( _window* w ) : _lua_gadget( w ) , _lua_interface_input( w )
		{ }
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_window>::FunctionType methods[];
		static Lunar<_lua_window>::PropertyType properties[];
	
};

#endif