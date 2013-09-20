#include "_lua/lua.gadget.textbox.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##           Lua-Textbox          ##
##################################*/

_lua_textbox::_lua_textbox( lua_State* L ) : 
	_lua_gadget( 
		new _textbox( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) , lightcheck<string>( L , 4 , "" ) , lightcheck<_style>( L , 6 , _style() ) )
	)
	, _lua_interface_input( (_textbox*)_lua_gadget::getGadget() )
{ }


//! setCursor
int _lua_textbox::setCursor( lua_State* L ){ _lua_interface_input::input->setCursor( check<int>( L , 1 ) ); return 0; }

//! getCursor
int _lua_textbox::getCursor( lua_State* L ){ lua_pushnumber( L , _lua_interface_input::input->getCursor() ); return 1; }

//! Lua-window
const char _lua_textbox::className[] = "TextBox";
Lunar<_lua_textbox>::FunctionType _lua_textbox::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_textbox>::PropertyType _lua_textbox::properties[] = {
	GADGET_BASE_ATTR,
	{ "text" , &_lua_textbox::getStrValue , &_lua_textbox::setStrValue },
	{ "color" , &_lua_textbox::getColor , &_lua_textbox::setColor },
	{ "cursor" , &_lua_textbox::getCursor , &_lua_textbox::setCursor },
	{ "bgColor" , &_lua_textbox::getBgColor , &_lua_textbox::setBgColor },
	{ "font" , &_lua_textbox::getFont , &_lua_textbox::setFont },
	{ "fontSize" , &_lua_textbox::getFontSize , &_lua_textbox::setFontSize },
	LUA_CLASS_ATTR_END
};