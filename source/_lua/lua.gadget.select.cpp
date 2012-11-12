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
Lunar<_lua_select>::FunctionType _lua_select::methods[] = {
	GADGET_FUNCS( _lua_select ),
	LUA_CLASS_FUNC(_lua_select,addIndex),
	LUA_CLASS_FUNC(_lua_select,removeIndex),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_select>::PropertyType _lua_select::properties[] = {
	GADGET_ATTRS( _lua_select ),
	LUA_CLASS_ATTR(_lua_select,IntValue,"intValue"),
	LUA_CLASS_ATTR_RD(_lua_select,StrValue,"strValue"),
	LUA_CLASS_ATTR_END
};