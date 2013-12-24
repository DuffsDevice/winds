#include "_lua/lua.gadget.window.bar.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##          Lua-WindowBar         ##
##################################*/

_lua_windowbar::_lua_windowbar( lua_State* L ) : 
	_lua_gadget( new _windowBar( optcheck<_length>( L , 1 ) , optcheck<_pixel>( L , 2 ) , lightcheck<_style>( L , 3 ) ) )
{ }

//! Lua-window
const char _lua_windowbar::className[] = "WindowBar";
Lunar<_lua_windowbar>::FunctionType _lua_windowbar::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_windowbar>::PropertyType _lua_windowbar::properties[] = {
	{ "bgColor" , wrap( _lua_windowbar , &_windowBar::getBgColor ) , wrap( _lua_windowbar , &_windowBar::setBgColor ) },
	LUA_CLASS_ATTR_END
};