#include "_lua/lua.class.ini.file.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*#############################
##           Lua-Ini         ##
#############################*/

_lua_inifile::_lua_inifile( lua_State* L ) :
	_lua_ini( new _iniFile( check<string>( L , 1 ) ) , true )
{}

_lua_inifile::_lua_inifile( _iniFile* i , bool wasAllocted ) :
	_lua_ini( i , wasAllocted )
{}

_lua_inifile::_lua_inifile( const _iniFile* i ) :
	_lua_ini( new _iniFile( *i ) , true )
{}

//! Lua-_ini
const char _lua_inifile::className[] = "IniFile";
Lunar<_lua_inifile>::FunctionType _lua_inifile::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_inifile>::PropertyType _lua_inifile::properties[] = {
	LUA_CLASS_ATTR_END
};