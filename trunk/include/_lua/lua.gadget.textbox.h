// Check if already included
#ifndef _WIN_LUATEXTBOX_
#define _WIN_LUATEXTBOX_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.textbox.h"

/**
 * Proxy Classes
**/
class _lua_textbox : public _lua_gadget , public _lua_interface_input<_textbox> {
	
	public:
		
		_lua_textbox( lua_State* L );
		
		_lua_textbox( _textbox* t ) : _lua_gadget( t ) , _lua_interface_input( t ) { }
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_textbox>::FunctionType methods[];
		static Lunar<_lua_textbox>::PropertyType properties[];
	
};

#endif