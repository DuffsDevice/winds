#include "_lua/lua.gadget.textbox.h"

/*##################################
##           Lua-Textbox          ##
##################################*/

_lua_textbox::_lua_textbox( lua_State* L ) : 
	_lua_gadget( 
		new _textbox( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_optstring( L , 4 , "" ) , luaL_optstyle( L , 5 ) )
	)
	, _lua_interface_input( (_textbox*)_lua_gadget::getGadget() )
{ }


//! setCursor
int _lua_textbox::setCursor( lua_State* L ){ _lua_interface_input::input->setCursor( luaL_checkint( L , 1 ) ); return 0; }

//! getCursor
int _lua_textbox::getCursor( lua_State* L ){ lua_pushnumber( L , _lua_interface_input::input->getCursor() ); return 1; }

//! Lua-window
const char _lua_textbox::className[] = "_textbox";
Lunar<_lua_textbox>::FunctionType _lua_textbox::methods[] = {
	GADGET_FUNCS( _lua_textbox ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_textbox>::PropertyType _lua_textbox::properties[] = {
	GADGET_ATTRS( _lua_textbox ),
	{ "text" , &_lua_textbox::getStrValue , &_lua_textbox::setStrValue },
	{ "color" , &_lua_textbox::getColor , &_lua_textbox::setColor },
	{ "cursor" , &_lua_textbox::getCursor , &_lua_textbox::setCursor },
	{ "bgColor" , &_lua_textbox::getBgColor , &_lua_textbox::setBgColor },
	{ "font" , &_lua_textbox::getFont , &_lua_textbox::setFont },
	{ "fontSize" , &_lua_textbox::getFontSize , &_lua_textbox::setFontSize },
	LUA_CLASS_ATTR_END
};