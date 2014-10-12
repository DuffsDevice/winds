#include <_lua/lua.class.menu.h>
#include <_lua/lua.class.menu.rule.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*##################################
##             Lua-Menu           ##
##################################*/

_lua_menu::_lua_menu( lua_State* L ) :
	menu( new _menu() )
{ }

int _lua_menu::addHandler(lua_State* L){
	_lua_menurule* rule = Lunar<_lua_menurule>::check( L , 1 );
	if( rule )
		menu->addHandler( *rule , check<_callback<_menuHandler>>( L , 2 ) );
	return 0;
}

int _lua_menu::getList(lua_State* L){
	return push( L , menu->getList( lightcheck<_u16>( L , 1 , 0 ) ) );
}

int _lua_menu::setList(lua_State* L){
	if( is_a<_u16>( L , 1 ) )
		menu->setList( check<_u16>( L , 1 ) , check<_menuEntryList>( L , 1 ) );
	else
		menu->setList( check<_menuEntryList>( L , 1 ) );
	return 0;
}

//! Lua-window
const char _lua_menu::className[] = "Menu";
Lunar<_lua_menu>::FunctionType _lua_menu::methods[] = {
	{ "getList"			, &_lua_menu::getList },
	{ "setList"			, &_lua_menu::setList },
	{ "addHandler"		, &_lua_menu::addHandler },
	{ "clearMenu"		, wrap( _lua_menu , &_menu::clearMenu ) },
	{ "clearHandlers"	, wrap( _lua_menu , &_menu::clearHandlers ) },
	{ "callHandler"		, wrap( _lua_menu , &_menu::callHandler ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_menu>::PropertyType _lua_menu::properties[] = {
	LUA_CLASS_ATTR_END
};