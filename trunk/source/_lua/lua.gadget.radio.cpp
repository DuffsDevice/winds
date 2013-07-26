#include "_lua/lua.gadget.radio.h"
#include "_lua/lua.class.radiogroup.h"

/*##################################
##            Lua-Radio           ##
##################################*/

_lua_radio::_lua_radio( lua_State* L ) : 
	_lua_gadget( new _radio( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , Lunar<_lua_radiogroup>::check( L , 3 ) , luaL_optint( L , 4 , -1 ) , luaL_optstyle( L , 5 ) ) )
	, _lua_interface_input( (_radio*)_lua_gadget::getGadget() )
{ }

//! Lua-button
const char _lua_radio::className[] = "_radio";
Lunar<_lua_radio>::FunctionType _lua_radio::methods[] = {
	GADGET_FUNCS( _lua_radio ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_radio>::PropertyType _lua_radio::properties[] = {
	GADGET_ATTRS( _lua_radio ),
	{ "checked" , &_lua_radio::getIntValue , &_lua_radio::setIntValue },
	LUA_CLASS_ATTR_END
};