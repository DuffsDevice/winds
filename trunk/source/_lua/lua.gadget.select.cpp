#include "_lua/lua.gadget.select.h"

/*##################################
##          Lua-Selectbox         ##
##################################*/

_lua_select::_lua_select( lua_State* L ) : 
	_lua_gadget( new _select( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) ) )
	, _lua_interface_input( (_interface_input*)nullptr )
{ 
	_lua_interface_input::input = (_select*)_lua_gadget::gadget;
}

//! addIndex
int _lua_select::addIndex( lua_State* L ){ 
	_select* s = ((_select*)_lua_gadget::gadget);
	s->getList()[ luaL_checkint( L , 1 ) ] = luaL_checkstring( L , 2 );
	s->refreshList();
	return 0;
}

//! removeIndex
int _lua_select::removeIndex( lua_State* L ){
	_select* s = ((_select*)_lua_gadget::gadget);
	s->getList().erase( luaL_checkint( L , 1 ) );
	s->refreshList();
	return 0;
}

//! Lua-button
const char _lua_select::className[] = "_select";
Lunar<_lua_select>::RegType _lua_select::methods[] = {
	LUNAR_DECLARE_ALL_METHODS_OF( _lua_select ),
	
	//! _interface_input (only these two are used)
	LUNAR_DECLARE_METHOD(_lua_select,addIndex),
	LUNAR_DECLARE_METHOD(_lua_select,removeIndex),
	LUNAR_DECLARE_METHOD(_lua_select,setIntValue),
	LUNAR_DECLARE_METHOD(_lua_select,getIntValue),
	LUNAR_DECLARE_METHOD(_lua_select,getStrValue),
	{0,0}
};