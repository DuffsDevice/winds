#include "_lua/lua.gadget.window.mainframe.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##          Lua-MainFrame         ##
##################################*/

_lua_mainframe::_lua_mainframe( lua_State* L ){
	luaL_error( L , "Not constructible in Lua. Use System.getMainFrame instead" );
}

//! Lua-window
const char _lua_mainframe::className[] = "MainFrame";
Lunar<_lua_mainframe>::FunctionType _lua_mainframe::methods[] = {
	{ "hasIcon"		, wrap( _lua_mainframe , &_mainFrame::hasIcon ) },
	{ "maximize"	, wrap( _lua_mainframe , &_mainFrame::maximize ) },
	{ "unMaximize"	, wrap( _lua_mainframe , &_mainFrame::unMaximize ) },
	{ "minimize"	, wrap( _lua_mainframe , &_mainFrame::minimize ) },
	{ "restore"		, wrap( _lua_mainframe , &_mainFrame::restore ) },
	{ "close"		, wrap( _lua_mainframe , &_mainFrame::close ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_mainframe>::PropertyType _lua_mainframe::properties[] = {
	{ "title"			, wrap( _lua_mainframe , &_window::getStrValue )	, wrap( _lua_mainframe , &_window::setStrValue ) },
	{ "icon"			, wrap( _lua_mainframe , &_window::getIcon )		, wrap( _lua_mainframe , &_window::setIcon ) },
	{ "minimized"		, wrap( _lua_mainframe , &_window::isMinimized )	, nullptr },
	{ "maximized"		, wrap( _lua_mainframe , &_window::isMaximized )	, nullptr },
	LUA_CLASS_ATTR_END
};