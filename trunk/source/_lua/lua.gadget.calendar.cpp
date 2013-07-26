#include "_lua/lua.gadget.calendar.h"

/*##################################
##           Lua-Calendar         ##
##################################*/

_lua_calendar::_lua_calendar( lua_State* L )
{
	// _calendar( _length width , _length height , _coord x , _coord y , _u16 year , _u8 month , _u8 dayOfMonth , _style style = _style() );
	this->setGadget( new _calendar( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkint( L , 5 ) , luaL_checkint( L , 6 ) , luaL_checkint( L , 7 ) , luaL_optstyle( L , 8 ) ) );
	
	_lua_interface_input::input = (_calendar*)_lua_gadget::getGadget();
}

//! Lua-button
const char _lua_calendar::className[] = "_calendar";
Lunar<_lua_calendar>::FunctionType _lua_calendar::methods[] = {
	GADGET_FUNCS( _lua_calendar ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_calendar>::PropertyType _lua_calendar::properties[] = {
	GADGET_ATTRS( _lua_calendar ),
	{ "selectedDate" , &_lua_calendar::getIntValue , &_lua_calendar::setIntValue },
	LUA_CLASS_ATTR_END
};