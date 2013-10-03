#include "_lua/lua.gadget.checkbox.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##          Lua-Checkbox          ##
##################################*/

_lua_checkbox::_lua_checkbox( lua_State* L ) :
	_lua_gadget( new _checkbox( check<int>( L , 1 ) , check<int>( L , 2 ) , lightcheck( L , 3 , _style() ) ) )
{}


//! Lua-window
const char _lua_checkbox::className[] = "CheckBox";
Lunar<_lua_checkbox>::FunctionType _lua_checkbox::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_checkbox>::PropertyType _lua_checkbox::properties[] = {
	GADGET_BASE_ATTR,
	{ "checked" , &_lua_checkbox::getIntValue , &_lua_checkbox::setIntValue },
	LUA_CLASS_ATTR_END
};