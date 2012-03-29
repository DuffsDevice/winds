// Check if already included
#ifndef _WIN_LUACHECKBOX_
#define _WIN_LUACHECKBOX_

extern "C"{
	#include "_lua/lua.h"
	#include "_lua/lauxlib.h"
	#include "_lua/lualib.h"
}
#include "_lua/lua.class.gadgetEventArgs.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.checkbox.h"
#include "_lua/lunar.h"

/**
 * Proxy Classes
**/
class _lua_checkbox : public _lua_gadget , public _lua_interface_input {
	
	public:
		
		_lua_checkbox( lua_State* L );
		
		_lua_checkbox( _checkbox* c ) : _lua_gadget( c ) , _lua_interface_input( c ) { }
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_checkbox>::RegType methods[];
	
};

#endif