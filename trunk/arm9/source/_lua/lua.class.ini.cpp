#include <_lua/lua.class.ini.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*#############################
##           Lua-Ini         ##
#############################*/

_lua_ini::_lua_ini( lua_State* ) :
	ini( new _ini )
	, wasAllocated( true )
{}

_lua_ini::_lua_ini( _ini* i , bool wasAllocated ) :
	ini( i ),
	wasAllocated( wasAllocated )
{}

_lua_ini::_lua_ini( const _ini* i ) :
	ini( new _ini( *i ) ),
	wasAllocated( true )
{}

//! Lua-_ini
const char _lua_ini::className[] = "Ini";
Lunar<_lua_ini>::FunctionType _lua_ini::methods[] = {
	{ "read"			, wrap( _lua_ini , &_ini::read ) },
	{ "write"			, wrap( _lua_ini , &_ini::write ) },
	{ "readIndex"		, wrap( _lua_ini , &_ini::readIndex ) },
	{ "readIndexInt"	, wrap( _lua_ini , &_ini::readIndexInt ) },
	{ "deleteSection"	, wrap( _lua_ini , &_ini::deleteSection ) },
	{ "deleteIndex"		, wrap( _lua_ini , &_ini::deleteIndex ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_ini>::PropertyType _lua_ini::properties[] = {
	LUA_CLASS_ATTR_END
};