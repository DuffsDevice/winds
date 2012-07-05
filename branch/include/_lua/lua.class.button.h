// Check if already included
#ifndef _WIN_LUABUTTON_
#define _WIN_LUABUTTON_

extern "C"{
	#include "_lua/lua.h"
	#include "_lua/lauxlib.h"
	#include "_lua/lualib.h"
}
#include "_lua/lua.class.gadgetEventArgs.h"
#include "_lua/lua.class.inputInterface.h"
#include "_lua/lunar.h"
#include "_gadget/gadget.button.h"

/**
 * Proxy Classes
**/
class _lua_button : public _lua_gadget , public _lua_interface_input{	
	
	public:
		
		_lua_button( lua_State* L );
		
		_lua_button( _button* b ) : _lua_gadget( b ) , _lua_interface_input( b ) { }
		
		//! setAutoSelect
		int setAutoSelect( lua_State* L );
		
		//! isAutoSelect
		int isAutoSelect( lua_State* L );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_button>::RegType methods[];
	
};

#endif