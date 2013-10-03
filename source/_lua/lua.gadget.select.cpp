#include "_lua/lua.gadget.select.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##           Lua-Select           ##
##################################*/

_lua_select::_lua_select( lua_State* L ) :
	_lua_gadget( new _select( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) , check<int>( L , 4 ) , lightcheck<_s32,string>( L , 5 , {} ) , lightcheck<int>( L , 6 , -1 ) , lightcheck<_style>( L , 7 , _style() ) ) )
{}

//! addIndex
int _lua_select::addIndex( lua_State* L ){
	getDerived()->getList()[ check<int>( L , 1 ) ] = check<string>( L , 2 );
	getDerived()->update();
	return 0;
}

//! removeIndex
int _lua_select::removeIndex( lua_State* L ){
	getDerived()->getList().erase( check<int>( L , 1 ) );
	getDerived()->update();
	return 0;
}

//! clearList
int _lua_select::clearList( lua_State* L ){
	getDerived()->getList().clear();
	getDerived()->update();
	return 0;
}

//! getList
int _lua_select::getList( lua_State* L )
{
	push( L , getDerived()->getList() );
	
	return 1;
}

//! setList
int _lua_select::setList( lua_State* L )
{	
	getDerived()->setList( lightcheck<_s32,string>( L , 5 , {} ) );
	
	return 0;
}

//! getEntryFromNumber
int _lua_select::getEntryFromNumber( lua_State* L ){
	lua_pushstring( L , getDerived()->getList()[check<int>(L,1)].c_str() );
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