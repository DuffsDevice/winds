#include "_lua/lua.gadget.calendar.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##           Lua-Calendar         ##
##################################*/

_lua_calendar::_lua_calendar( lua_State* L )
{
	// _calendar( _length width , _length height , _coord x , _coord y , _u16 year , _u8 month , _u8 dayOfMonth , _style style = _style() );
	this->setGadget( new _calendar( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) , check<int>( L , 4 ) , check<int>( L , 5 ) , check<int>( L , 6 ) , check<int>( L , 7 ) , lightcheck<_pixel>( L , 8 , COLOR_WHITE ) , lightcheck( L , 8 , _style() ) ) );
	
	_lua_interface_input::input = (_calendar*)_lua_gadget::getGadget();
}

//! Lua-button
const char _lua_calendar::className[] = "Calendar";
Lunar<_lua_calendar>::FunctionType _lua_calendar::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_calendar>::PropertyType _lua_calendar::properties[] = {
	GADGET_BASE_ATTR,
	{ "selectedDate" , &_lua_calendar::getIntValue , &_lua_calendar::setIntValue },
	{ "bgColor" , &_lua_calendar::getBgColor , &_lua_calendar::setBgColor },
	LUA_CLASS_ATTR_END
};