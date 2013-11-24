#include "_lua/lua.gadget.textarea.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##          Lua-Textarea          ##
##################################*/

_lua_textarea::_lua_textarea( lua_State* L ) : 
	_lua_gadget( 
		new _textarea( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , optcheck<int>( L , 4 ) , lightcheck<string>( L , 5 ) , lightcheck<_style>( L , 6 ) )
	)
{}

//! Lua-window
const char _lua_textarea::className[] = "TextArea";
Lunar<_lua_textarea>::FunctionType _lua_textarea::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_textarea>::PropertyType _lua_textarea::properties[] = {
	GADGET_BASE_ATTR,
	{ "text"		, wrap( _lua_textarea , &_textarea::getStrValue )	, wrap( _lua_textarea , &_textarea::setStrValue ) },
	{ "color"		, wrap( _lua_textarea , &_textarea::getColor )		, wrap( _lua_textarea , &_textarea::setColor ) },
	{ "bgColor"		, wrap( _lua_textarea , &_textarea::getBgColor )	, wrap( _lua_textarea , &_textarea::setBgColor ) },
	{ "cursor"		, wrap( _lua_textarea , &_textarea::getCursor )		, wrap( _lua_textarea , &_textarea::setCursor ) },
	{ "font"		, wrap( _lua_textarea , &_textarea::getFont )		, wrap( _lua_textarea , &_textarea::setFont ) },
	{ "fontSize"	, wrap( _lua_textarea , &_textarea::getFontSize )	, wrap( _lua_textarea , &_textarea::setFontSize ) },
	LUA_CLASS_ATTR_END
};