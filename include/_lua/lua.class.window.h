// Check if already included
#ifndef _WIN_LUAWINDOW_
#define _WIN_LUAWINDOW_

extern "C"{
	#include "_lua/lua.h"
	#include "_lua/lauxlib.h"
	#include "_lua/lualib.h"
}
#include "_lua/lua.class.gadgetEventArgs.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.window.h"
#include "_lua/lunar.h"

/**
 * Proxy Classes
**/
class _lua_window : public _lua_gadget , public _lua_interface_input{	
	
	public:
		
		_lua_window( lua_State* L );
		
		_lua_window( _window* w ) : _lua_gadget( w ) , _lua_interface_input( w ) { }
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_window>::RegType methods[];
	
};

#endif