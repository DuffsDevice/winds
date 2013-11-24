#include "_lua/lua.class.menu.h"
#include "_lua/lua.class.menu.rule.h"
#include "_lua/lua.func.h"
#include "_type/type.callback.derives.h"
using namespace _luafunc;

/*##################################
##             Lua-Menu           ##
##################################*/

_lua_menu::_lua_menu( lua_State* L ) : 
	_menu()
{ }

int _lua_menu::addMenuHandler(lua_State* L){
	_menuHandlerRule* rule = Lunar<_lua_menurule>::check( L , 1 );
	if( rule )
		_menu::addMenuHandler( *rule , _luaCallback<_menuHandler>( L , 2 ) );
	return 0;
}

int _lua_menu::setMainList(lua_State* L){
	_menu::setMainList( lightcheck<_menuEntryList>( L , 1 ) );
	return 0;
}

int _lua_menu::getMainList(lua_State* L){
	return push( L , _menu::getMainList() );
}

int _lua_menu::clear(lua_State* L){
	_menu::clear();
	return 0;
}

int _lua_menu::setList(lua_State* L){
	_menu::setList( check<int>( L , 1 ) , lightcheck<_menuEntryList>( L , 2 ) );
	return 0;
}

int _lua_menu::getList(lua_State* L){
	return push( L , _menu::getList( check<int>( L , 1 ) ) );
}

int _lua_menu::callHandler(lua_State* L){
	_menu::callHandler( check<int>( L , 1 ) , check<int>( L , 2 ) );
	return 0;
}

//! Lua-window
const char _lua_menu::className[] = "Menu";
Lunar<_lua_menu>::FunctionType _lua_menu::methods[] = {
	LUA_CLASS_FUNC( _lua_menu , addMenuHandler ) ,
	LUA_CLASS_FUNC( _lua_menu , getList ) ,
	LUA_CLASS_FUNC( _lua_menu , setList ) ,
	LUA_CLASS_FUNC( _lua_menu , clear ) ,
	LUA_CLASS_FUNC( _lua_menu , callHandler ) ,
	LUA_CLASS_FUNC_END
};

Lunar<_lua_menu>::PropertyType _lua_menu::properties[] = {
	{ "mainList" , &_lua_menu::getMainList , &_lua_menu::setMainList },
	LUA_CLASS_ATTR_END
};