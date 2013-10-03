#include "_lua/lua.gadget.contextmenu.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##           Lua-Select           ##
##################################*/

_lua_contextmenu::_lua_contextmenu( lua_State* L ) : 
	_lua_gadget( new _contextMenu( lightcheck<_s32,string>( L , 1 , {} ) , lightcheck<_gadget>( L , 2 , nullptr ) , lua_isnumber( L , 3 ) ? _optValue<_length>( check<int>( L , 3 ) ) : _optValue<_length>() , lightcheck<bool>( L , 4 , false ) , lightcheck<int>( L , 5 , -1 ) , lightcheck<_style>( L , 6 , _style() ) ) )
{}

//! addIndex
int _lua_contextmenu::addIndex( lua_State* L ){
	getDerived()->getList()[ check<int>( L , 1 ) ] = check<string>( L , 2 );
	getDerived()->update();
	return 0;
}

//! removeIndex
int _lua_contextmenu::removeIndex( lua_State* L ){
	getDerived()->getList().erase( check<int>( L , 1 ) );
	getDerived()->update();
	return 0;
}

//! clearList
int _lua_contextmenu::clearList( lua_State* L ){
	getDerived()->getList().clear();
	getDerived()->update();
	return 0;
}

//! getList
int _lua_contextmenu::getList( lua_State* L )
{
	push( L , getDerived()->getList() );
	
	return 1;
}

//! setList
int _lua_contextmenu::setList( lua_State* L )
{
	getDerived()->setList( lightcheck<_s32,string>( L , 5 , {} ) );
	
	return 0;
}

//! getEntryFromNumber
int _lua_contextmenu::getEntryFromNumber( lua_State* L ){
	lua_pushstring( L , getDerived()->getList()[check<int>(L,1)].c_str() );
	return 1;
}

//! Lua-button
const char _lua_contextmenu::className[] = "Select";
Lunar<_lua_contextmenu>::FunctionType _lua_contextmenu::methods[] = {
	LUA_CLASS_FUNC(_lua_contextmenu,addIndex),
	LUA_CLASS_FUNC(_lua_contextmenu,removeIndex),
	LUA_CLASS_FUNC(_lua_contextmenu,clearList),
	LUA_CLASS_FUNC(_lua_contextmenu,getEntryFromNumber),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_contextmenu>::PropertyType _lua_contextmenu::properties[] = {
	GADGET_BASE_ATTR,
	{ "strValue" , &_lua_contextmenu::getStrValue , nullptr },
	{ "value" , &_lua_contextmenu::getIntValue , &_lua_contextmenu::setIntValue },
	{ "list" , &_lua_contextmenu::getList , &_lua_contextmenu::setList },
	LUA_CLASS_ATTR_END
};