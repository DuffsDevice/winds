#include "_lua/lua.gadget.counter.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##           Lua-Counter          ##
##################################*/

_lua_counter::_lua_counter( lua_State* L ) :
	_lua_gadget( new _counter( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , lightcheck<bool>( L , 4 , false ) , lightcheck<int>( L , 5 , 0 ) , optcheck<int>( L , 6 ) , optcheck<int>( L , 7 ) , optcheck<int>( L , 8 ) , lightcheck<_style>( L , 9 , _style() ) ) )
{}

//! Lua-button
const char _lua_counter::className[] = "Counter";
Lunar<_lua_counter>::FunctionType _lua_counter::methods[] = {
	{ "increase"	, wrap( _lua_counter , &_counter::increase ) },
	{ "decrease"	, wrap( _lua_counter , &_counter::decrease ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_counter>::PropertyType _lua_counter::properties[] = {
	{ "value" 		, wrap( _lua_counter , &_counter::getIntValue )		, wrap( _lua_counter , &_counter::setIntValue )  },
	{ "upperBound" 	, wrap( _lua_counter , &_counter::getUpperBound )	, wrap( _lua_counter , &_counter::setUpperBound )  },
	{ "lowerBound" 	, wrap( _lua_counter , &_counter::getLowerBound )	, wrap( _lua_counter , &_counter::setLowerBound )  },
	LUA_CLASS_ATTR_END
};