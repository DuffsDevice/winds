#include "_lua/lua.class.animation.h"
#include "_type/type.callback.derives.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
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

//! setEasing
int _lua_animation::setEasing(lua_State* L){ _animation::setEasing( string2easingFunc[ check<string>( L , 1 ) ] ); return 1; }

//! terminate
int _lua_animation::terminate(lua_State* L){ _animation::terminate( lightcheck<bool>( L , 1 , false ) ); return 0; }

//! getEasing
int _lua_animation::getEasing(lua_State* L){ lua_pushstring( L , easingFunc2string[ _animation::getEasing() ] ); return 1; }


//! Lua-_rect
const char _lua_animation::className[] = "Animation";
Lunar<_lua_animation>::FunctionType _lua_animation::methods[] = {
	{ "setSetter"		, wrap( _lua_animation , &_animation::setSetter ) },
	{ "setFinisher"		, wrap( _lua_animation , &_animation::setFinisher ) },
	{ "deleteFinisher"	, wrap( _lua_animation , &_animation::deleteFinisher ) },
	{ "deleteSetter"	, wrap( _lua_animation , &_animation::deleteSetter ) },
	{ "start"			, wrap( _lua_animation , &_animation::start ) },
	{ "terminate"		, &_lua_animation::terminate },
	{ "pause"			, wrap( _lua_animation , &_animation::pause ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_animation>::PropertyType _lua_animation::properties[] = {
	{ "fromValue"	, wrap( _lua_animation , &_animation::getFromValue )	, wrap( _lua_animation , &_animation::setFromValue ) },
	{ "toValue"		, wrap( _lua_animation , &_animation::getToValue )		, wrap( _lua_animation , &_animation::setToValue ) },
	{ "duration"	, wrap( _lua_animation , &_animation::getDuration )		, wrap( _lua_animation , &_animation::setDuration ) },
	{ "runs"		, wrap( _lua_animation , &_animation::isRunning )		, 0 },
	{ "easing"		, &_lua_animation::getEasing							, &_lua_animation::setEasing },
	LUA_CLASS_ATTR_END
};