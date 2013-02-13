#include "_lua/lua.gadget.window.h"

/*##################################
##           Lua-Window           ##
##################################*/

_lua_window::_lua_window( lua_State* L ) : 
	_lua_gadget( 
		new _window( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkstring( L , 5 ) , luaL_optstyle( L , 6 ) )
	)
	, _lua_interface_input( (_window*)_lua_gadget::gadget )
{ }


//! Lua-window
const char _lua_window::className[] = "_window";
Lunar<_lua_window>::FunctionType _lua_window::methods[] = {
	GADGET_FUNCS( _lua_window ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_window>::PropertyType _lua_window::properties[] = {
	GADGET_ATTRS( _lua_window ),
	{ "strValue" , &_lua_window::getStrValue , &_lua_window::setStrValue },
	LUA_CLASS_ATTR_END
};