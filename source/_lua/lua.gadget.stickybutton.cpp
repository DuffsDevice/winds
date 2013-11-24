#include "_lua/lua.gadget.stickybutton.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##         Lua-Stickybutton       ##
##################################*/

_lua_stickybutton::_lua_stickybutton( lua_State* L ) :
	_lua_gadget( new _stickybutton( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , optcheck<int>( L , 4 ) , lightcheck<string>( L , 5 ) , lightcheck<_style>( L , 6 ) ) )
{}


//! Lua-button
const char _lua_stickybutton::className[] = "StickyButton";
Lunar<_lua_stickybutton>::FunctionType _lua_stickybutton::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_stickybutton>::PropertyType _lua_stickybutton::properties[] = {
	GADGET_BASE_ATTR,
	{ "checked"		, wrap( _lua_stickybutton , &_stickybutton::getIntValue )	, wrap( _lua_stickybutton , &_stickybutton::setIntValue ) },
	{ "text"		, wrap( _lua_stickybutton , &_stickybutton::getStrValue )	, wrap( _lua_stickybutton , &_stickybutton::setStrValue ) },
	{ "autoSelect"	, wrap( _lua_stickybutton , &_stickybutton::isAutoSelect )	, wrap( _lua_stickybutton , &_stickybutton::setAutoSelect ) },
	{ "font"		, wrap( _lua_stickybutton , &_stickybutton::getFont )		, wrap( _lua_stickybutton , &_stickybutton::setFont ) },
	{ "fontSize"	, wrap( _lua_stickybutton , &_stickybutton::getFontSize )	, wrap( _lua_stickybutton , &_stickybutton::setFontSize ) }
};