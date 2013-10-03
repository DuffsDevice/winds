#include "_lua/lua.gadget.radio.h"
#include "_lua/lua.class.radiogroup.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##            Lua-Radio           ##
##################################*/

_lua_radio::_lua_radio( lua_State* L ) : 
	_lua_gadget( new _radio( check<int>( L , 1 ) , check<int>( L , 2 ) , Lunar<_lua_radiogroup>::check( L , 3 ) , lightcheck<int>( L , 4 , -1 ) , lightcheck<_style>( L , 5 , _style() ) ) )
{ }

//! Lua-button
const char _lua_radio::className[] = "Radio";
Lunar<_lua_radio>::FunctionType _lua_radio::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_radio>::PropertyType _lua_radio::properties[] = {
	GADGET_BASE_ATTR,
	{ "checked" , &_lua_radio::getIntValue , &_lua_radio::setIntValue },
	LUA_CLASS_ATTR_END
};