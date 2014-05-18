#include "_lua/lua.class.menu.h"
#include "_lua/lua.class.menu.rule.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##             Lua-Menu           ##
##################################*/

_lua_menu::_lua_menu( lua_State* L ) : 
	_menu()
{ }

int _lua_menu::addMenuHandler(lua_State* L){
	_lua_menurule* rule = Lunar<_lua_menurule>::check( L , 1 );
	if( rule )
		_menu::addMenuHandler( *rule , check<_callback<_menuHandler>>( L , 2 ) );
	return 0;
}

int _lua_menu::clear(lua_State* L){
	_menu::clear();
	return 0;
}

int _lua_menu::callHandler(lua_State* L){
	_menu::callHandler( check<int>( L , 1 ) , check<int>( L , 2 ) );
	return 0;
}


using typedefHelper1 = const _menuEntryList&(_menu::*)(_int)const;
using typedefHelper2 = const _menuEntryList&(_menu::*)()const;

//! Lua-window
const char _lua_menu::className[] = "Menu";
Lunar<_lua_menu>::FunctionType _lua_menu::methods[] = {
	LUA_CLASS_FUNC( _lua_menu , addMenuHandler ) ,
	{ "getList"		, wrap( _lua_menu , (typedefHelper1)&_menu::getList ) },
	{ "setList"		, wrap( _lua_menu , &_menu::setList ) },
	{ "clear"		, wrap( _lua_menu , &_menu::clear ) },
	{ "callHandler"	, wrap( _lua_menu , &_menu::callHandler ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_menu>::PropertyType _lua_menu::properties[] = {
	{ "mainList" , wrap( _lua_menu , (typedefHelper2)&_menu::getMainList ) , wrap( _lua_menu , &_menu::setMainList ) },
	LUA_CLASS_ATTR_END
};