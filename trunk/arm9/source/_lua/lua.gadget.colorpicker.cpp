#include "_lua/lua.gadget.colorpicker.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##         Lua-Colorpicker        ##
##################################*/

_lua_colorpicker::_lua_colorpicker( lua_State* L ) :
	_lua_gadget( new _colorPicker( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , optcheck<int>( L , 4 ) , lightcheck<_pixel>( L , 5 , COLOR_BLUE ) , lightcheck<_style>( L , 6 , _style() ) ) )
{}

//! Lua-button
const char _lua_colorpicker::className[] = "ColorPicker";
Lunar<_lua_colorpicker>::FunctionType _lua_colorpicker::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_colorpicker>::PropertyType _lua_colorpicker::properties[] = {
	{ "color" , wrap( _lua_colorpicker , &_colorPicker::getColor ) , wrap( _lua_colorpicker , &_colorPicker::setColor ) },
	LUA_CLASS_ATTR_END
};