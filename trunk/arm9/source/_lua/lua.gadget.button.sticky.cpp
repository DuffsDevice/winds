#include <_lua/lua.gadget.button.sticky.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*##################################
##         Lua-Stickybutton       ##
##################################*/

_lua_stickybutton::_lua_stickybutton( lua_State* L ) :
	_lua_gadget( new _stickyButton( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , optcheck<int>( L , 4 ) , lightcheck<string>( L , 5 ) , lightcheck<_style>( L , 6 ) ) )
{}


//! Lua-button
const char _lua_stickybutton::className[] = "StickyButton";
Lunar<_lua_stickybutton>::FunctionType _lua_stickybutton::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_stickybutton>::PropertyType _lua_stickybutton::properties[] = {
	{ "checked"		, wrap( _lua_stickybutton , &_stickyButton::getIntValue )	, wrap( _lua_stickybutton , &_stickyButton::setIntValue ) } ,
	{ "autoSelect"	, nullptr													, nullptr }
};