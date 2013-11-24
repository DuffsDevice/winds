#include "_lua/lua.class.timer.h"
#include "_lua/lua.func.h"
#include "_type/type.callback.derives.h"
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

int _lua_timer::setCallback(lua_State* L){ _timer::setCallback( _luaCallback<void()>( L , 1 ) ); return 0; }

int _lua_timer::deleteCallback(lua_State* L){ _timer::deleteCallback(); return 0; }

int _lua_timer::start(lua_State* L){ _timer::start(); return 0; }

int _lua_timer::stop(lua_State* L){ _timer::stop(); return 0; }

int _lua_timer::getRepeating(lua_State* L){ push( L , _timer::isRepeating() ); return 1; }

int _lua_timer::getDuration(lua_State* L){ push( L , _timer::getDuration() ); return 1; }

int _lua_timer::setRepeating(lua_State* L){ _timer::setRepeating( check<bool>( L , 1 ) ); return 0; }

int _lua_timer::setDuration(lua_State* L){ _timer::setDuration( check<int>( L , 1 ) ); return 0; }

//! Lua-_timer
const char _lua_timer::className[] = "Timer";
Lunar<_lua_timer>::FunctionType _lua_timer::methods[] = {
  LUA_CLASS_FUNC(_lua_timer, setCallback),
  LUA_CLASS_FUNC(_lua_timer, deleteCallback),
  LUA_CLASS_FUNC(_lua_timer, start),
  LUA_CLASS_FUNC(_lua_timer, stop),
  LUA_CLASS_FUNC_END
};

Lunar<_lua_timer>::PropertyType _lua_timer::properties[] = {
	{ "repeating" , &_lua_timer::getRepeating , &_lua_timer::setRepeating },
	{ "duration" , &_lua_timer::getDuration , &_lua_timer::setDuration },
	LUA_CLASS_ATTR_END
};