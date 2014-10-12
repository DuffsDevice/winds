#include <_lua/lua.gadget.radio.h>
#include <_lua/lua.class.radiogroup.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*##################################
##            Lua-Radio           ##
##################################*/

_lua_radio::_lua_radio( lua_State* L ) : 
	_lua_gadget( new _radio( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , Lunar<_lua_radiogroup>::check( L , 3 ) , lightcheck<int>( L , 4 , -1 ) , lightcheck<_style>( L , 5 ) ) )
{ }

//! Lua-button
const char _lua_radio::className[] = "Radio";
Lunar<_lua_radio>::FunctionType _lua_radio::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_radio>::PropertyType _lua_radio::properties[] = {
	{ "checked" , wrap( _lua_radio , &_radio::getIntValue ) , wrap( _lua_radio , &_radio::setIntValue ) },
	LUA_CLASS_ATTR_END
};