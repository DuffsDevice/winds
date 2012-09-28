#include "_lua/lua.gadget.checkbox.h"

/*##################################
##          Lua-Checkbox          ##
##################################*/

_lua_checkbox::_lua_checkbox( lua_State* L ) :
	_lua_interface_input( new _checkbox( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) )  )
{ 
	this->setGadget( (_checkbox*)_lua_interface_input::input );
}


//! Lua-window
const char _lua_checkbox::className[] = "_checkbox";

Lunar<_lua_checkbox>::RegType _lua_checkbox::methods[] = {
	LUNAR_DECLARE_ALL_METHODS_OF( _lua_checkbox ),
	
	//! _interface_input (only these two are used)
	LUNAR_DECLARE_METHOD(_lua_checkbox,setTitle),
	LUNAR_DECLARE_METHOD(_lua_checkbox,getTitle),
	LUNAR_DECLARE_METHOD(_lua_checkbox,getIntValue),
	LUNAR_DECLARE_METHOD(_lua_checkbox,setIntValue),
	{0,0}
};