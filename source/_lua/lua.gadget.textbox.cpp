#include "_lua/lua.gadget.textbox.h"
//#include "_lua/lua.class.font.h"

/*##################################
##            Lua-Label           ##
##################################*/

_lua_textbox::_lua_textbox( lua_State* L ) : 
	_lua_gadget( 
		new _textbox( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkstring( L , 4 ) )
	)
	, _lua_interface_input( (_textbox*)_lua_gadget::gadget )
{ }

//! Lua-window
const char _lua_textbox::className[] = "_textbox";
Lunar<_lua_textbox>::FunctionType _lua_textbox::methods[] = {
	GADGET_FUNCS( _lua_textbox ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_textbox>::PropertyType _lua_textbox::properties[] = {
	GADGET_ATTRS( _lua_textbox ),
	{ "strValue" , &_lua_textbox::getStrValue , &_lua_textbox::setStrValue },
	{ "color" , &_lua_textbox::getColor , &_lua_textbox::setColor },
	{ "bgColor" , &_lua_textbox::getBgColor , &_lua_textbox::setBgColor },
	{ "font" , &_lua_textbox::getFont , &_lua_textbox::setFont },
	{ "fontSize" , &_lua_textbox::getFontSize , &_lua_textbox::setFontSize },
	LUA_CLASS_ATTR_END
};