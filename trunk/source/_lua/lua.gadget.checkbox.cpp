#include "_lua/lua.gadget.checkbox.h"

/*##################################
##          Lua-Checkbox          ##
##################################*/

_lua_checkbox::_lua_checkbox( lua_State* L ) :
	_lua_interface_input( new _checkbox( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_optstyle( L , 3 ) )  )
{ 
	this->setGadget( (_checkbox*)_lua_interface_input::input );
}


//! Lua-window
const char _lua_checkbox::className[] = "_checkbox";
Lunar<_lua_checkbox>::FunctionType _lua_checkbox::methods[] = {
	GADGET_FUNCS( _lua_checkbox ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_checkbox>::PropertyType _lua_checkbox::properties[] = {
	GADGET_ATTRS( _lua_checkbox ),
	//! _interface_input (only these two are used)
	{ "intValue" , &_lua_checkbox::getIntValue , &_lua_checkbox::setIntValue },
	LUA_CLASS_ATTR_END
};