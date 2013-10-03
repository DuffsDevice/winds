// Check if already included
#ifndef _WIN_L_CHECKBOX_
#define _WIN_L_CHECKBOX_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.checkbox.h"

/**
 * Proxy Classes
 */
class _lua_checkbox : public _lua_gadget , public _lua_interface_input<_checkbox> {
	
	public:
		
		_lua_checkbox( lua_State* L );
		
		_lua_checkbox( _checkbox* c ) : _lua_gadget( c , false )
		{ }
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_checkbox>::FunctionType methods[];
		static Lunar<_lua_checkbox>::PropertyType properties[];
	
};

#endif