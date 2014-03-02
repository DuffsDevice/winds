// Check if already included
#ifndef _WIN_L_COLORPICKER_GRADIENT_
#define _WIN_L_COLORPICKER_GRADIENT_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.colorpicker.gradient.h"

/**
 * Proxy Classes
 */
class _lua_gradientcolorpicker : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_gradientcolorpicker( lua_State* L );
		
		//! C-Ctor
		_lua_gradientcolorpicker( _gradientColorPicker* c ) : _lua_gadget( c , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_gradientcolorpicker>::FunctionType methods[];
		static Lunar<_lua_gradientcolorpicker>::PropertyType properties[];
		using baseclasses = Lunar<_lua_gradientcolorpicker>::BaseClassType<_lua_gadget>;
};

#endif