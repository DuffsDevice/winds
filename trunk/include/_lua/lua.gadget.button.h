// Check if already included
#ifndef _WIN_LUABUTTON_
#define _WIN_LUABUTTON_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.button.h"

/**
 * Proxy Classes
**/
class _lua_button : public _lua_gadget , public _lua_interface_input<_button> {
	
	public:
		
		_lua_button( lua_State* L );
		
		_lua_button( _button* b ){ 
			this->setGadget( b );
			_lua_interface_input::input = (_button*)this->gadget;
		}
		
		//! setAutoSelect
		int setAutoSelect( lua_State* L );
		
		//! isAutoSelect
		int isAutoSelect( lua_State* L );
		
		//! getFont
		int getFont( lua_State* L );
		
		//! getFont
		int setFont( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_button>::FunctionType methods[];
		static Lunar<_lua_button>::PropertyType properties[];
	
};

#endif