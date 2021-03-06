#include <_lua/lua.gadget.progressbar.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*##################################
##         Lua-Progressbar        ##
##################################*/

_lua_progressbar::_lua_progressbar( lua_State* L ) :
	_lua_gadget( 
		new _progressBar(
			optcheck<int>( L , 1 )
			, optcheck<int>( L , 2 )
			, optcheck<int>( L , 3 )
			, lightcheck<bool>( L , 4 , true )
			, lightcheck<_style>( L , 5 )
		)
	)
{}

//! Lua-button
const char _lua_progressbar::className[] = "ProgressBar";
Lunar<_lua_progressbar>::FunctionType _lua_progressbar::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_progressbar>::PropertyType _lua_progressbar::properties[] = {
	{ "value" 		, wrap( _lua_progressbar , &_progressBar::getIntValue )		, wrap( _lua_progressbar , &_progressBar::setIntValue ) },
	{ "colorScheme" , wrap( _lua_progressbar , &_progressBar::getColorScheme )	, wrap( _lua_progressbar , &_progressBar::setColorScheme ) },
	{ "barType" 	, wrap( _lua_progressbar , &_progressBar::getBarType )		, wrap( _lua_progressbar , &_progressBar::setBarType ) },
	LUA_CLASS_ATTR_END
};