#include "_lua/lua.gadget.window.menu.h"
#include "_lua/lua.class.menu.h"
#include "_lua/lua.func.h"
using namespace _luafunc;

/*##################################
##         Lua-WindowMenu         ##
##################################*/

_lua_windowmenu::_lua_windowmenu( lua_State* L ) : 
	_lua_gadget()
{
	_lua_menu* menu = Lunar<_lua_menu>::lightcheck( L , 1 );
	if( menu )
		setGadget( new _windowMenu( *menu , lightcheck<_style>( L , 2 ) ) );
	else
		setGadget( new _windowMenu( _menu() , lightcheck<_style>( L , 2 ) ) );
}

int _lua_windowmenu::getStandardMenu( lua_State* L ){
	Lunar<_lua_menu>::push( L , new _lua_menu( _windowMenu::getStandardMenu() ) );
	return 1;
}

//! Lua-Name
const char _lua_windowmenu::className[] = "WindowMenu";
Lunar<_lua_windowmenu>::FunctionType _lua_windowmenu::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_windowmenu>::PropertyType _lua_windowmenu::properties[] = {
	GADGET_BASE_ATTR,
	LUA_CLASS_ATTR_END
};

Lunar<_lua_windowmenu>::StaticType	_lua_windowmenu::staticmethods[] = {
	{ "getStandardMenu" , &_lua_windowmenu::getStandardMenu },
	LUA_CLASS_FUNC_END
};