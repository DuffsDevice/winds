#include <_lua/lua.gadget.clock.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*##################################
##           Lua-Button           ##
##################################*/

_lua_clockgadget::_lua_clockgadget( lua_State* L ) :
	_lua_gadget(
		/*if*/ is_a<_time>( L , 4 ) ?
			new _clockGadget(
				optcheck<int>( L , 1 )
				, optcheck<int>( L , 2 )
				, optcheck<int>( L , 3 )
				, optcheck<int>( L , 4 )
				, check<_time>( L , 5 )
				, lightcheck<bool>( L , 6 , true )
				, lightcheck<_style>( L , 7 )
			)
		: /*else*/
			new _clockGadget(
				optcheck<int>( L , 1 )
				, optcheck<int>( L , 2 )
				, optcheck<int>( L , 3 )
				, optcheck<int>( L , 4 )
				, check<int>( L , 5 )
				, check<int>( L , 6 )
				, check<int>( L , 7 )
				, lightcheck<bool>( L , 8 , true )
				, lightcheck<_style>( L , 9 )
			)
	)
{}

//! Lua-button
const char _lua_clockgadget::className[] = "Clock";
Lunar<_lua_clockgadget>::FunctionType _lua_clockgadget::methods[] = {
	{ "increase" , wrap( _lua_clockgadget , &_clockGadget::increase ) },
	{ "decrease" , wrap( _lua_clockgadget , &_clockGadget::decrease ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_clockgadget>::PropertyType _lua_clockgadget::properties[] = {
	{ "outerBgColor"	, wrap( _lua_clockgadget , &_clockGadget::getOuterBgColor )		, wrap( _lua_clockgadget , &_clockGadget::setOuterBgColor ) },
	{ "innerBgColor"	, wrap( _lua_clockgadget , &_clockGadget::getInnerBgColor )		, wrap( _lua_clockgadget , &_clockGadget::setInnerBgColor ) },
	{ "secondColor"		, wrap( _lua_clockgadget , &_clockGadget::getSecondsColor )		, wrap( _lua_clockgadget , &_clockGadget::setSecondsColor ) },
	{ "minuteColor"		, wrap( _lua_clockgadget , &_clockGadget::getMinutesColor )		, wrap( _lua_clockgadget , &_clockGadget::setMinutesColor ) },
	{ "hourColor"		, wrap( _lua_clockgadget , &_clockGadget::getSecondsColor )		, wrap( _lua_clockgadget , &_clockGadget::setSecondsColor ) },
	{ "markingColor"	, wrap( _lua_clockgadget , &_clockGadget::getMarkingsColor )	, wrap( _lua_clockgadget , &_clockGadget::setMarkingsColor ) },
	{ "autoIncrease"	, wrap( _lua_clockgadget , &_clockGadget::isAutoIncreasing )	, wrap( _lua_clockgadget , &_clockGadget::setAutoIncrease ) },
	LUA_CLASS_ATTR_END
};