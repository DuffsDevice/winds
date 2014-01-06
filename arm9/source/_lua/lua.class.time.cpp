#include "_lua/lua.class.time.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
#include "_type/type.callback.derives.h"
using namespace _luafunc;

/*##################################
##            Lua-Timer           ##
##################################*/

//! Constructor
_lua_time::_lua_time( lua_State* L ) : 
	_time( is_a<int>( L , 1 ) ? _time( check<int>( L , 1 ) ) : _time() )
{}

int _lua_time::now( lua_State* L ){
	return push( L , _time::now() );
}

int _lua_time::date( lua_State* L ){
	return push( L , _time::date( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) ) );
}

int _lua_time::time( lua_State* L ){
	return push( L , _time::time( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) ) );
}

//! Lua-_time
const char _lua_time::className[] = "Time";
Lunar<_lua_time>::FunctionType _lua_time::methods[] = {
  { "get"			, wrap( _lua_time , &_time::get ) },
  { "set"			, wrap( _lua_time , &_time::set ) },
  { "add"			, wrap( _lua_time , &_time::add ) },
  { "toUnixTime"	, wrap( _lua_time , &_time::toUnixTime ) },
  { "toString"		, wrap( _lua_time , &_time::toString ) },
  LUA_CLASS_FUNC_END
};

Lunar<_lua_time>::PropertyType _lua_time::properties[] = {
	LUA_CLASS_ATTR_END
};

Lunar<_lua_time>::StaticType	_lua_time::staticmethods[] = {
	{ "now" , &_lua_time::now },
	{ "date" , &_lua_time::date },
	{ "time" , &_lua_time::time },
	LUA_CLASS_FUNC_END
};