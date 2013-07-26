#include "_lua/lua.gadget.label.h"

/*##################################
##            Lua-Label           ##
##################################*/

_lua_label::_lua_label( lua_State* L ) : 
	_lua_gadget( 
		new _label( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkstring( L , 3 ) , luaL_optstyle( L , 4 ) )
	)
	, _lua_interface_input( (_label*)_lua_gadget::getGadget() )
{ }

//! Lua-window
const char _lua_label::className[] = "_label";
Lunar<_lua_label>::FunctionType _lua_label::methods[] = {
	GADGET_FUNCS( _lua_label ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_label>::PropertyType _lua_label::properties[] = {
	GADGET_ATTRS( _lua_label ),
	//! _interface_input (only these two are used)
	{ "text" , &_lua_label::getStrValue , &_lua_label::setStrValue },
	{ "color" , &_lua_label::getColor , &_lua_label::setColor },
	{ "bgColor" , &_lua_label::getBgColor , &_lua_label::setBgColor },
	{ "font" , &_lua_label::getFont , &_lua_label::setFont },
	{ "fontSize" , &_lua_label::getFontSize , &_lua_label::setFontSize },
	LUA_CLASS_ATTR_END
};