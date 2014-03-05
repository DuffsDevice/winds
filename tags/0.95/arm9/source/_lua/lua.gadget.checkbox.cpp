#include "_lua/lua.gadget.checkbox.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##          Lua-Checkbox          ##
##################################*/

_lua_checkbox::_lua_checkbox( lua_State* L ) :
	_lua_gadget( new _checkbox( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , lightcheck<_style>( L , 3 , _style() ) ) )
{}


//! Lua-window
const char _lua_checkbox::className[] = "CheckBox";
Lunar<_lua_checkbox>::FunctionType _lua_checkbox::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_checkbox>::PropertyType _lua_checkbox::properties[] = {
	{ "checked" , wrap( _lua_checkbox , &_checkbox::getIntValue ) , wrap( _lua_checkbox , &_checkbox::setIntValue ) },
	LUA_CLASS_ATTR_END
};