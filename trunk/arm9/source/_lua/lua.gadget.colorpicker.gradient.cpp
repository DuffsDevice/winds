#include <_lua/lua.gadget.colorpicker.gradient.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*##################################
##         Lua-Colorpicker        ##
##################################*/

_lua_gradientcolorpicker::_lua_gradientcolorpicker( lua_State* L ) :
	_lua_gadget( new _gradientColorPicker( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , optcheck<int>( L , 4 ) , lightcheck<_color>( L , 5 , _color::blue ) , lightcheck<_style>( L , 6 , _style() ) ) )
{}

//! Lua-button
const char _lua_gradientcolorpicker::className[] = "GradientColorPicker";
Lunar<_lua_gradientcolorpicker>::FunctionType _lua_gradientcolorpicker::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_gradientcolorpicker>::PropertyType _lua_gradientcolorpicker::properties[] = {
	{ "color" , wrap( _lua_gradientcolorpicker , &_gradientColorPicker::getColor ) , wrap( _lua_gradientcolorpicker , &_gradientColorPicker::setColor ) },
	LUA_CLASS_ATTR_END
};