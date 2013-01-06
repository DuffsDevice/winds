// Check if already included
#ifndef _WIN_LUACHECKBOX_
#define _WIN_LUACHECKBOX_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.checkbox.h"

/**
 * Proxy Classes
**/
class _lua_checkbox : public _lua_gadget , public _lua_interface_input<_checkbox> {
	
	public:
		
		_lua_checkbox( lua_State* L );
		
		_lua_checkbox( _checkbox* c ){ 
			this->setGadget( c );
			_lua_interface_input::input = (_checkbox*)this->gadget;
		}
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_checkbox>::FunctionType methods[];
		static Lunar<_lua_checkbox>::PropertyType properties[];
	
};

#endif