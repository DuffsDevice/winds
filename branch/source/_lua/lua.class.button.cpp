#include "_lua/lua.class.button.h"

/*##################################
##           Lua-Button           ##
##################################*/

_lua_button::_lua_button( lua_State* L ) : 
	_lua_gadget( (_gadget*)nullptr ) , _lua_interface_input( (_interface_input*)nullptr )
{ 
	if( lua_gettop( L ) == 3 )
		_lua_gadget::gadget = new _button( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkstring( L , 3 ) );
	else
		_lua_gadget::gadget = new _button( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkstring( L , 5 ) );
	
	_lua_interface_input::input = (_button*)_lua_gadget::gadget;
}

//! setAutoSelect
int _lua_button::setAutoSelect( lua_State* L ){ ((_button*)_lua_gadget::gadget)->setAutoSelect( luaL_optint( L , 1 , true ) ); return 0; };

//! isAutoSelect
int _lua_button::isAutoSelect( lua_State* L ){ lua_pushboolean( L , ((_button*)_lua_gadget::gadget)->isAutoSelect() ); return 1; }

//! Lua-button
const char _lua_button::className[] = "_button";
Lunar<_lua_button>::RegType _lua_button::methods[] = {
	LUNAR_DECLARE_ALL_METHODS_OF( _lua_button ),
	
	//! _interface_input (only these two are used)
	LUNAR_DECLARE_METHOD(_lua_button,setTitle),
	LUNAR_DECLARE_METHOD(_lua_button,getTitle),
	LUNAR_DECLARE_METHOD(_lua_button,setAutoSelect),
	LUNAR_DECLARE_METHOD(_lua_button,isAutoSelect),
	{0,0}
};