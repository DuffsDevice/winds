// Check if already included
#ifndef _WIN_L_RADIO_
#define _WIN_L_RADIO_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.radio.h"

/**
 * Proxy Classes
 */
class _lua_radio : public _lua_gadget , public _lua_interface_input<_radio> {
	
	public:
		
		_lua_radio( lua_State* L );
		
		_lua_radio( _radio* b ) : _lua_gadget( b , false ) , _lua_interface_input( b )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_radio>::FunctionType methods[];
		static Lunar<_lua_radio>::PropertyType properties[];
	
};

#endif