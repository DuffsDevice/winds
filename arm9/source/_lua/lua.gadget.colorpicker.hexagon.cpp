#include "_lua/lua.gadget.colorpicker.hexagon.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##         Lua-Colorpicker        ##
##################################*/

_lua_hexagoncolorpicker::_lua_hexagoncolorpicker( lua_State* L ) :
	_lua_gadget( new _hexagonColorPicker( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , lightcheck<_color>( L , 3 , _color::white ) , lightcheck<_style>( L , 4 , _style() ) ) )
{}

//! Lua-button
const char _lua_hexagoncolorpicker::className[] = "HexagonColorPicker";
Lunar<_lua_hexagoncolorpicker>::FunctionType _lua_hexagoncolorpicker::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_hexagoncolorpicker>::PropertyType _lua_hexagoncolorpicker::properties[] = {
	{ "color"	, wrap( _lua_hexagoncolorpicker , &_hexagonColorPicker::getColor )		, wrap( _lua_hexagoncolorpicker , &_hexagonColorPicker::setColor ) },
	{ "bgColor"	, wrap( _lua_hexagoncolorpicker , &_hexagonColorPicker::getBgColor )	, wrap( _lua_hexagoncolorpicker , &_hexagonColorPicker::setBgColor ) },
	LUA_CLASS_ATTR_END
};