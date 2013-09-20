#include "_lua/lua.gadget.select.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##           Lua-Select           ##
##################################*/

_lua_select::_lua_select( lua_State* L ) : 
	_lua_gadget( new _select( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) , check<int>( L , 4 ) , lightcheck<_s32,string>( L , 5 , {} ) , lightcheck<int>( L , 6 , -1 ) , lightcheck<_style>( L , 7 , _style() ) ) )
	, _lua_interface_input( (_select*)_lua_gadget::getGadget() )
{
}

//! addIndex
int _lua_select::addIndex( lua_State* L ){
	_lua_interface_input::input->getList()[ check<int>( L , 1 ) ] = check<string>( L , 2 );
	_lua_interface_input::input->update();
	return 0;
}

//! removeIndex
int _lua_select::removeIndex( lua_State* L ){
	_lua_interface_input::input->getList().erase( check<int>( L , 1 ) );
	_lua_interface_input::input->update();
	return 0;
}

//! clearList
int _lua_select::clearList( lua_State* L ){
	_lua_interface_input::input->getList().clear();
	_lua_interface_input::input->update();
	return 0;
}

//! getList
int _lua_select::getList( lua_State* L )
{
	push( L , _lua_interface_input::input->getList() );
	
	return 1;
}

//! setList
int _lua_select::setList( lua_State* L )
{	
	_lua_interface_input::input->setList( lightcheck<_s32,string>( L , 5 , {} ) );
	
	return 0;
}

//! getEntryFromNumber
int _lua_select::getEntryFromNumber( lua_State* L ){
	lua_pushstring( L , _lua_interface_input::input->getList()[check<int>(L,1)].c_str() );
	return 1;
}

//! Lua-button
const char _lua_select::className[] = "Select";
Lunar<_lua_select>::FunctionType _lua_select::methods[] = {
	LUA_CLASS_FUNC(_lua_select,addIndex),
	LUA_CLASS_FUNC(_lua_select,removeIndex),
	LUA_CLASS_FUNC(_lua_select,clearList),
	LUA_CLASS_FUNC(_lua_select,getEntryFromNumber),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_select>::PropertyType _lua_select::properties[] = {
	GADGET_BASE_ATTR,
	{ "strValue" , &_lua_select::getStrValue , nullptr },
	{ "value" , &_lua_select::getIntValue , &_lua_select::setIntValue },
	{ "list" , &_lua_select::getList , &_lua_select::setList },
	LUA_CLASS_ATTR_END
};