#include "_lua/lua.gadget.window.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

#include "_type/type.system.h"

/*##################################
##           Lua-Window           ##
##################################*/

_lua_window::_lua_window( lua_State* L ) : 
	_lua_gadget()
{
	if( is_a<_bitmap>( L , 6 ) )
	{
		this->setGadget(
			new _window(
				optcheck<int>( L , 1 )
				, optcheck<int>( L , 2 )
				, optcheck<int>( L , 3 )
				, optcheck<int>( L , 4 )
				, check<string>( L , 5 )
				, check<_bitmap>( L , 6 )
				, lightcheck<bool>( L , 7 , true )
				, lightcheck<bool>( L , 8 , true )
				, lightcheck<_style>( L , 9 , _style() | _styleAttr::draggable )
			) 
		);
	}
	else
	{
		this->setGadget(
			new _window(
				optcheck<int>( L , 1 )
				, optcheck<int>( L , 2 )
				, optcheck<int>( L , 3 )
				, optcheck<int>( L , 4 )
				, check<string>( L , 5 )
				, lightcheck<bool>( L , 6 , true )
				, lightcheck<bool>( L , 7 , true )
				, lightcheck<_style>( L , 8 , _style() | _styleAttr::draggable ) 
			)
		);
	}
}

//! Lua-window
const char _lua_window::className[] = "Window";
Lunar<_lua_window>::FunctionType _lua_window::methods[] = {
	{ "hasIcon"		, wrap( _lua_window , &_window::hasIcon ) },
	{ "maximize"	, wrap( _lua_window , &_window::maximize ) },
	{ "unMaximize"	, wrap( _lua_window , &_window::unMaximize ) },
	{ "minimize"	, wrap( _lua_window , &_window::minimize ) },
	{ "restore"		, wrap( _lua_window , &_window::restore ) },
	{ "close"		, wrap( _lua_window , &_window::close ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_window>::PropertyType _lua_window::properties[] = {
	{ "title"			, wrap( _lua_window , &_window::getStrValue )		, wrap( _lua_window , &_window::setStrValue ) },
	{ "icon"			, wrap( _lua_window , &_window::getIcon )			, wrap( _lua_window , &_window::setIcon ) },
	{ "minimizeable"	, wrap( _lua_window , &_window::isMinimizeable )	, wrap( _lua_window , &_window::setMinimizeable ) },
	{ "closeable"		, wrap( _lua_window , &_window::isCloseable )		, wrap( _lua_window , &_window::setCloseable ) },
	{ "minimized"		, wrap( _lua_window , &_window::isMinimized )		, nullptr },
	{ "maximized"		, wrap( _lua_window , &_window::isMaximized )		, nullptr },
	{ "isTask"			, wrap( _lua_window , &_window::isTask )			, nullptr },
	LUA_CLASS_ATTR_END
};