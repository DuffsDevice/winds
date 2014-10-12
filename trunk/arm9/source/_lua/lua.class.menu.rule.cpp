#include <_lua/lua.class.menu.rule.h>
#include <_lua/lua.func.h>
using namespace _luafunc;

/*##################################
##             Lua-Menu           ##
##################################*/

_lua_menurule::_lua_menurule( lua_State* L ) : 
	_menuHandlerRule(
		/* if */ lua_gettop( L ) >= 3 ?
			_menuHandlerRule( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) )
		:
		/* else if */ lua_gettop( L ) >= 2 ?
			_menuHandlerRule( check<int>( L , 1 ) , check<int>( L , 2 ) )
		:
		/* else if */ lua_gettop( L ) >= 1 ?
			_menuHandlerRule( check<int>( L , 1 ) )
		:
		/* else */
			_menuHandlerRule()
	)
{ }

//! Lua-window
const char _lua_menurule::className[] = "MenuRule";
Lunar<_lua_menurule>::FunctionType _lua_menurule::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_menurule>::PropertyType _lua_menurule::properties[] = {
	LUA_CLASS_ATTR_END
};