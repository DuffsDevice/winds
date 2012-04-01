#include "_lua/lua.class.label.h"

/*##################################
##            Lua-Label           ##
##################################*/

_lua_label::_lua_label( lua_State* L ) : 
	_lua_gadget( 
		new _label( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkstring( L , 3 ) )
	)
	, _lua_interface_input( (_label*)_lua_gadget::gadget )
{ }


//! Lua-window
const char _lua_label::className[] = "_label";

Lunar<_lua_label>::RegType _lua_label::methods[] = {
	LUNAR_DECLARE_ALL_METHODS_OF( _lua_label ),
	
	//! _interface_input (only these two are used)
	LUNAR_DECLARE_METHOD(_lua_label,getStrValue),
	LUNAR_DECLARE_METHOD(_lua_label,setStrValue),
	{0,0}
};