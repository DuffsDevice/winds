#include "_lua/lua.gadget.progressbar.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##         Lua-Progressbar        ##
##################################*/

_lua_progressbar::_lua_progressbar( lua_State* L )
	: _lua_gadget( 
		new _progressbar(
			check<int>( L , 1 )
			, check<int>( L , 2 )
			, check<int>( L , 3 )
			, lightcheck<bool>( L , 4 , true )
			, lightcheck<_style>( L , 5 , _style() )
		)
	)
	, _lua_interface_input( (_progressbar*)_lua_gadget::getGadget() )
{ }

//! setColorScheme
int _lua_progressbar::setColorScheme( lua_State* L ){ _lua_interface_input::input->setColorScheme( check<bool>( L , 1 ) ); return 0; };

//! getColorScheme
int _lua_progressbar::getColorScheme( lua_State* L ){ lua_pushboolean( L , _lua_interface_input::input->getColorScheme() ); return 1; }

//! setBarType
int _lua_progressbar::setBarType( lua_State* L ){ _lua_interface_input::input->setBarType( check<bool>( L , 1 ) ); return 0; };

//! getBarType
int _lua_progressbar::getBarType( lua_State* L ){ lua_pushboolean( L , _lua_interface_input::input->getColorScheme() ); return 1; }

//! Lua-button
const char _lua_progressbar::className[] = "ProgressBar";
Lunar<_lua_progressbar>::FunctionType _lua_progressbar::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_progressbar>::PropertyType _lua_progressbar::properties[] = {
	GADGET_BASE_ATTR,
	{ "value" , &_lua_progressbar::getIntValue , &_lua_progressbar::setIntValue },
	{ "colorScheme" , &_lua_progressbar::getColorScheme , &_lua_progressbar::setColorScheme },
	{ "barType" , &_lua_progressbar::getBarType , &_lua_progressbar::setBarType },
	LUA_CLASS_ATTR_END
};