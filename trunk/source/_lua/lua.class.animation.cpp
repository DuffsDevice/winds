#include "_lua/lua.class.animation.h"
#include "_lua/lua.func.h"
#include "_type/type.callback.derives.h"
using namespace _luafunc;

/*##################################
##           Lua-Border           ##
##################################*/

_lua_animation::_lua_animation( _animation rc ) : _animation( rc )
{}

//! Constructor
_lua_animation::_lua_animation( lua_State* L ) : 
	_animation( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) )
{ }

//! setFromValue
int _lua_animation::setFromValue(lua_State* L){ _animation::setFromValue( check<int>( L , 1 ) ); return 0; }

//! setToValue
int _lua_animation::setToValue(lua_State* L){ _animation::setToValue( check<int>( L , 1 ) ); return 0; }

//! setDuration
int _lua_animation::setDuration(lua_State* L){ _animation::setDuration( check<int>( L , 1 ) ); return 0; }

//! setSetter
int _lua_animation::setSetter(lua_State* L){ _animation::setter( _luaCallback<void(int)>( L , 1 ) ); return 0; }

//! setFinish
int _lua_animation::setFinish(lua_State* L){ _animation::finish( _luaCallback<void(int)>( L , 1 ) ); return 0; }

//! setEasing
int _lua_animation::setEasing(lua_State* L){ _animation::setEasing( string2easingFunc[ check<string>( L , 1 ) ] ); return 1; }

//! getFromValue
int _lua_animation::getFromValue(lua_State* L){ lua_pushnumber( L , _animation::getFromValue() ); return 1; }

//! getToValue
int _lua_animation::getToValue(lua_State* L){ lua_pushnumber( L , _animation::getToValue() ); return 1; }

//! getDuration
int _lua_animation::getDuration(lua_State* L){ lua_pushnumber( L , _animation::getDuration() ); return 1; }

//! getEasing
int _lua_animation::getEasing(lua_State* L){ lua_pushstring( L , easingFunc2string[ _animation::getEasing() ].c_str() ); return 1; }

//! getRunning
int _lua_animation::getRunning(lua_State* L){ lua_pushboolean( L , _animation::isRunning() ); return 1; }

//! start
int _lua_animation::start(lua_State* L){ _animation::start(); return 0; }

//! terminate
int _lua_animation::terminate(lua_State* L){ _animation::terminate( lightcheck<bool>( L , 1 , false ) ); return 0; }

//! pause
int _lua_animation::pause(lua_State* L){ _animation::pause(); return 0; }

//! Lua-_rect
const char _lua_animation::className[] = "Animation";
Lunar<_lua_animation>::FunctionType _lua_animation::methods[] = {
	{ "onSet" , &_lua_animation::setSetter },
	{ "onFinish" , &_lua_animation::setFinish },
	{ "start" , &_lua_animation::start },
	{ "terminate" , &_lua_animation::terminate },
	{ "pause" , &_lua_animation::pause },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_animation>::PropertyType _lua_animation::properties[] = {
	{ "fromValue" , &_lua_animation::getFromValue , &_lua_animation::setFromValue },
	{ "toValue" , &_lua_animation::getToValue , &_lua_animation::setToValue },
	{ "duration" , &_lua_animation::getDuration , &_lua_animation::setDuration },
	{ "easing" , &_lua_animation::getEasing , &_lua_animation::setEasing },
	{ "runs" , &_lua_animation::getRunning , 0 },
	LUA_CLASS_ATTR_END
};