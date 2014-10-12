#include <_lua/lua.class.timer.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*##################################
##            Lua-Timer           ##
##################################*/

//! Constructor
_lua_timer::_lua_timer( lua_State* L ) : 
	_timer( check<int>( L , lua_isfunction( L , 1 ) ? 2 : 1 ) , check<bool>( L , lua_isfunction( L , 1 ) ? 3 : 2 ) )
{
	if( lua_isfunction( L , 1 ) )
		_timer::setCallback( _luaCallback<void()>( L , 1 ) );
}

//! Lua-_timer
const char _lua_timer::className[] = "Timer";
Lunar<_lua_timer>::FunctionType _lua_timer::methods[] = {
  { "setCallback"		, wrap( _lua_timer , &_timer::setCallback ) },
  { "deleteCallback"	, wrap( _lua_timer , &_timer::deleteCallback ) },
  { "start"				, wrap( _lua_timer , &_timer::start ) },
  { "stop"				, wrap( _lua_timer , &_timer::stop ) },
  LUA_CLASS_FUNC_END
};

Lunar<_lua_timer>::PropertyType _lua_timer::properties[] = {
	{ "repeating"	, wrap( _lua_timer , &_timer::isRepeating )	, wrap( _lua_timer , &_timer::setRepeating ) },
	{ "duration"	, wrap( _lua_timer , &_timer::getDuration )	, wrap( _lua_timer , &_timer::setDuration ) },
	LUA_CLASS_ATTR_END
};