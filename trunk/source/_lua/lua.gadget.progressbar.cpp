#include "_lua/lua.gadget.progressbar.h"

/*##################################
##         Lua-Progressbar        ##
##################################*/

_lua_progressbar::_lua_progressbar( lua_State* L )
	: _lua_gadget( 
		new _progressbar(
			luaL_checkint( L , 1 )
			, luaL_checkint( L , 2 )
			, luaL_checkint( L , 3 )
			, luaL_optboolean( L , 4 , true )
			, luaL_optstyle( L , 5 )
		)
	)
	, _lua_interface_input( (_progressbar*)_lua_gadget::gadget )
{ }

//! setColorScheme
int _lua_progressbar::setColorScheme( lua_State* L ){ _lua_interface_input::input->setColorScheme( luaL_checkboolean( L , 1 ) ); return 0; };

//! getColorScheme
int _lua_progressbar::getColorScheme( lua_State* L ){ lua_pushboolean( L , _lua_interface_input::input->getColorScheme() ); return 1; }

//! setBarType
int _lua_progressbar::setBarType( lua_State* L ){ _lua_interface_input::input->setBarType( luaL_checkboolean( L , 1 ) ); return 0; };

//! getBarType
int _lua_progressbar::getBarType( lua_State* L ){ lua_pushboolean( L , _lua_interface_input::input->getColorScheme() ); return 1; }

//! Lua-button
const char _lua_progressbar::className[] = "_progressbar";
Lunar<_lua_progressbar>::FunctionType _lua_progressbar::methods[] = {
	GADGET_FUNCS( _lua_progressbar ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_progressbar>::PropertyType _lua_progressbar::properties[] = {
	GADGET_ATTRS( _lua_progressbar ),
	{ "intValue" , &_lua_progressbar::getIntValue , &_lua_progressbar::setIntValue },
	{ "colorScheme" , &_lua_progressbar::getColorScheme , &_lua_progressbar::setColorScheme },
	{ "barType" , &_lua_progressbar::getBarType , &_lua_progressbar::setBarType },
	LUA_CLASS_ATTR_END
};