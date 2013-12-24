#include "_lua/lua.gadget.button.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##           Lua-Button           ##
##################################*/

_lua_button::_lua_button( lua_State* L ) :
	_lua_gadget( new _button( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , optcheck<int>( L , 4 ) , lightcheck<string>( L , 5 , "" ) , lightcheck<_style>( L , 6 ) ) )
{}

//! Lua-button
const char _lua_button::className[] = "Button";
Lunar<_lua_button>::FunctionType _lua_button::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_button>::PropertyType _lua_button::properties[] = {
	{ "text"		, wrap( _lua_button , &_button::getStrValue )	, wrap( _lua_button , &_button::setStrValue ) },
	{ "autoSelect"	, wrap( _lua_button , &_button::isAutoSelect )	, wrap( _lua_button , &_button::setAutoSelect ) },
	{ "font"		, wrap( _lua_button , &_button::getFont )		, wrap( _lua_button , &_button::setFont ) },
	{ "fontSize"	, wrap( _lua_button , &_button::getFontSize )	, wrap( _lua_button , &_button::setFontSize ) },
	LUA_CLASS_ATTR_END
};