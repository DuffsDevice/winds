// Check if already included
#ifndef _WIN_L_RADIO_
#define _WIN_L_RADIO_

#include <_lua/lua.lunar.h>
#include <_lua/lua.class.gadget.h>
#include <_gadget/gadget.radio.h>

/**
 * Proxy Classes
 */
class _lua_radio : public _lua_gadget{
	
	public:
		
		_lua_radio( lua_State* L );
		
		_lua_radio( _radio* b ) : _lua_gadget( b , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_radio>::FunctionType methods[];
		static Lunar<_lua_radio>::PropertyType properties[];
		using baseclasses = Lunar<_lua_radio>::BaseClassType<_lua_gadget>;
};

#endif