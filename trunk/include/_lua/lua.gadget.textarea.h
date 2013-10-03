// Check if already included
#ifndef _WIN_L_TEXTAREA_
#define _WIN_L_TEXTAREA_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.textarea.h"

/**
 * Proxy Classes
 */
class _lua_textarea : public _lua_gadget , public _lua_interface_input<_textarea> {
	
	public:
		
		_lua_textarea( lua_State* L );
		
		_lua_textarea( _textarea* t ) : _lua_gadget( t , false )
		{ }
		
		//! setCursor
		int setCursor( lua_State* L );
		
		//! getCursor
		int getCursor( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_textarea>::FunctionType methods[];
		static Lunar<_lua_textarea>::PropertyType properties[];
	
};

#endif