#include "_lua/lua.gadget.label.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##            Lua-Label           ##
##################################*/

_lua_label::_lua_label( lua_State* L ) :
	_lua_gadget( new _label( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , optcheck<int>( L , 4 ) , check<string>( L , 5 ) , lightcheck<_style>( L , 6 , _style() ) ) )
{ }

//! Lua-window
const char _lua_label::className[] = "Label";
Lunar<_lua_label>::FunctionType _lua_label::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_label>::PropertyType _lua_label::properties[] = {
	GADGET_BASE_ATTR,
	{ "text" , &_lua_label::getStrValue , &_lua_label::setStrValue },
	{ "color" , &_lua_label::getColor , &_lua_label::setColor },
	{ "bgColor" , &_lua_label::getBgColor , &_lua_label::setBgColor },
	{ "font" , &_lua_label::getFont , &_lua_label::setFont },
	{ "fontSize" , &_lua_label::getFontSize , &_lua_label::setFontSize },
	LUA_CLASS_ATTR_END
};