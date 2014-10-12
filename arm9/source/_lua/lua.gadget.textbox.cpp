#include <_lua/lua.gadget.textbox.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*##################################
##           Lua-Textbox          ##
##################################*/

_lua_textbox::_lua_textbox( lua_State* L ) : 
	_lua_gadget( 
		new _textBox( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , optcheck<int>( L , 4 ) , lightcheck<string>( L , 5 , "" ) , lightcheck<_style>( L , 6 ) )
	)
{}

//! Lua-window
const char _lua_textbox::className[] = "TextBox";
Lunar<_lua_textbox>::FunctionType _lua_textbox::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_textbox>::PropertyType _lua_textbox::properties[] = {
	{ "text" 		, wrap( _lua_textbox , &_textBox::getStrValue )	, wrap( _lua_textbox , &_textBox::setStrValue ) },
	{ "color" 		, wrap( _lua_textbox , &_textBox::getColor )	, wrap( _lua_textbox , &_textBox::setColor ) },
	{ "bgColor" 	, wrap( _lua_textbox , &_textBox::getBgColor )	, wrap( _lua_textbox , &_textBox::setBgColor ) },
	{ "cursor" 		, wrap( _lua_textbox , &_textBox::getCursor )	, wrap( _lua_textbox , &_textBox::setCursor ) },
	{ "font" 		, wrap( _lua_textbox , &_textBox::getFont )		, wrap( _lua_textbox , &_textBox::setFont ) },
	{ "fontSize"	, wrap( _lua_textbox , &_textBox::getFontSize )	, wrap( _lua_textbox , &_textBox::setFontSize ) },
	LUA_CLASS_ATTR_END
};