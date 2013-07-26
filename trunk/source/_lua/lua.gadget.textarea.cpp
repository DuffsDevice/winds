#include "_lua/lua.gadget.textarea.h"

/*##################################
##          Lua-Textarea          ##
##################################*/

_lua_textarea::_lua_textarea( lua_State* L ) : 
	_lua_gadget( 
		new _textarea( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_optstring( L , 5 , "" ) , luaL_optstyle( L , 6 ) )
	)
	, _lua_interface_input( (_textarea*)_lua_gadget::getGadget() )
{ }


//! setCursor
int _lua_textarea::setCursor( lua_State* L ){ _lua_interface_input::input->setCursor( luaL_checkint( L , 1 ) ); return 0; }

//! getCursor
int _lua_textarea::getCursor( lua_State* L ){ lua_pushnumber( L , _lua_interface_input::input->getCursor() ); return 1; }

//! Lua-window
const char _lua_textarea::className[] = "_textarea";
Lunar<_lua_textarea>::FunctionType _lua_textarea::methods[] = {
	GADGET_FUNCS( _lua_textarea ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_textarea>::PropertyType _lua_textarea::properties[] = {
	GADGET_ATTRS( _lua_textarea ),
	{ "text" , &_lua_textarea::getStrValue , &_lua_textarea::setStrValue },
	{ "color" , &_lua_textarea::getColor , &_lua_textarea::setColor },
	{ "cursor" , &_lua_textarea::getCursor , &_lua_textarea::setCursor },
	{ "bgColor" , &_lua_textarea::getBgColor , &_lua_textarea::setBgColor },
	{ "font" , &_lua_textarea::getFont , &_lua_textarea::setFont },
	{ "fontSize" , &_lua_textarea::getFontSize , &_lua_textarea::setFontSize },
	LUA_CLASS_ATTR_END
};