#include "_lua/lua.gadget.window.h"

/*##################################
##           Lua-Window           ##
##################################*/

_lua_window::_lua_window( lua_State* L ) : 
	_lua_gadget( 
		new _window( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkstring( L , 5 ) )
	)
	, _lua_interface_input( (_window*)_lua_gadget::gadget )
{ }

int _lua_window::close( lua_State* L ){
	this->gadget->setParent( nullptr );
	return 0;
}


//! Lua-window
const char _lua_window::className[] = "_window";

Lunar<_lua_window>::RegType _lua_window::methods[] = {
	// All Derived Gadget-Methods
	LUNAR_DECLARE_ALL_METHODS_OF( _lua_window ),
	LUNAR_DECLARE_METHOD(_lua_window,close),
  
	//! _interface_input (only these two are used)
	LUNAR_DECLARE_METHOD(_lua_window,setTitle),
	LUNAR_DECLARE_METHOD(_lua_window,getTitle),
	{0,0}
};