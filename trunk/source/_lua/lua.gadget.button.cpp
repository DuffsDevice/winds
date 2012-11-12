#include "_lua/lua.gadget.button.h"

/*##################################
##           Lua-Button           ##
##################################*/

_lua_button::_lua_button( lua_State* L )
{ 
	if( lua_gettop( L ) == 3 )
		this->setGadget( new _button( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkstring( L , 3 ) ) );
	else
		this->setGadget( new _button( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkstring( L , 5 ) ) );
	
	_lua_interface_input::input = (_button*)_lua_gadget::gadget;
}

//! setAutoSelect
int _lua_button::setAutoSelect( lua_State* L ){ ((_button*)_lua_gadget::gadget)->setAutoSelect( luaL_optint( L , 1 , true ) ); return 0; };

//! isAutoSelect
int _lua_button::isAutoSelect( lua_State* L ){ lua_pushboolean( L , ((_button*)_lua_gadget::gadget)->isAutoSelect() ); return 1; }

//! Lua-button
const char _lua_button::className[] = "_button";
Lunar<_lua_button>::FunctionType _lua_button::methods[] = {
	GADGET_FUNCS( _lua_button ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_button>::PropertyType _lua_button::properties[] = {
	GADGET_ATTRS( _lua_button ),
	LUA_CLASS_ATTR( _lua_button , Title , "text" ),
	LUA_CLASS_ATTR_EX( _lua_button , isAutoSelect , setAutoSelect , "autoSelect" ),
	LUA_CLASS_ATTR_END
};