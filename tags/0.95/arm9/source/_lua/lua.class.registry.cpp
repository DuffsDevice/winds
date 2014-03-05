#include "_lua/lua.class.registry.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*#############################
##           Lua-Ini         ##
#############################*/

_lua_registry::_lua_registry( lua_State* L ) :
	_lua_inifile( new _registry( check<string>( L , 1 ) ) , true )
{}

_lua_registry::_lua_registry( _registry* i ) :
	_lua_inifile( i , false )
{}

_lua_registry::_lua_registry( const _registry* i ) :
	_lua_inifile( new _registry( *i ) , true )
{}

//! Lua-_ini
const char _lua_registry::className[] = "Ini";
Lunar<_lua_registry>::FunctionType _lua_registry::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_registry>::PropertyType _lua_registry::properties[] = {
	LUA_CLASS_ATTR_END
};