#include <_lua/lua.gadget.calendar.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*##################################
##           Lua-Calendar         ##
##################################*/

_lua_calendar::_lua_calendar( lua_State* L ) :
	_lua_gadget( new _calendar( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , optcheck<int>( L , 4 ) , check<int>( L , 5 ) , check<int>( L , 6 ) , check<int>( L , 7 ) , lightcheck<_color>( L , 8 , _color::white ) , lightcheck<_style>( L , 8 ) ) )
{}

//! Lua-button
const char _lua_calendar::className[] = "Calendar";
Lunar<_lua_calendar>::FunctionType _lua_calendar::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_calendar>::PropertyType _lua_calendar::properties[] = {
	{ "date" 	, wrap( _lua_calendar , &_calendar::getIntValue )	, wrap( _lua_calendar , &_calendar::setIntValue ) },
	{ "bgColor" , wrap( _lua_calendar , &_calendar::getBgColor )	, wrap( _lua_calendar , &_calendar::setBgColor ) },
	LUA_CLASS_ATTR_END
};