// Check if already included
#ifndef _WIN_L_COLORPICKER_
#define _WIN_L_COLORPICKER_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.colorpicker.h"

/**
 * Proxy Classes
 */
class _lua_colorpicker : public _lua_gadget , public _lua_interface_input<_colorpicker> {
	
	public:
		
		_lua_colorpicker( lua_State* L );
		
		_lua_colorpicker( _colorpicker* c ) : _lua_gadget( c , false ) , _lua_interface_input( c )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_colorpicker>::FunctionType methods[];
		static Lunar<_lua_colorpicker>::PropertyType properties[];
	
};

#endif