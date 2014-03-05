// Check if already included
#ifndef _WIN_L_COLORPICKER_HEXAGON_
#define _WIN_L_COLORPICKER_HEXAGON_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.colorpicker.hexagon.h"

/**
 * Proxy Classes
 */
class _lua_hexagoncolorpicker : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_hexagoncolorpicker( lua_State* L );
		
		//! C-Ctor
		_lua_hexagoncolorpicker( _hexagonColorPicker* c ) : _lua_gadget( c , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_hexagoncolorpicker>::FunctionType methods[];
		static Lunar<_lua_hexagoncolorpicker>::PropertyType properties[];
		using baseclasses = Lunar<_lua_hexagoncolorpicker>::BaseClassType<_lua_gadget>;
};

#endif