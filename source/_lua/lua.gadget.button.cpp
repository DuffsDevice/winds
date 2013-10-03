#include "_lua/lua.gadget.button.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##           Lua-Button           ##
##################################*/

_lua_button::_lua_button( lua_State* L ) :
	_lua_gadget( new _button( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , check<int>( L , 3 ) , check<int>( L , 4 ) , lightcheck<string>( L , 5 , "" ) , lightcheck( L , 6 , _style() ) ) )
{}

//! setAutoSelect
int _lua_button::setAutoSelect( lua_State* L ){	getDerived()->setAutoSelect( check<bool>( L , 1 ) ); return 0; };

//! isAutoSelect
int _lua_button::isAutoSelect( lua_State* L ){ lua_pushboolean( L , getDerived()->isAutoSelect() ); return 1; }

//! Lua-button
const char _lua_button::className[] = "Button";
Lunar<_lua_button>::FunctionType _lua_button::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_button>::PropertyType _lua_button::properties[] = {
	GADGET_BASE_ATTR,
	{ "text" , &_lua_button::getStrValue , &_lua_button::setStrValue },
	{ "autoSelect" , &_lua_button::isAutoSelect , &_lua_button::setAutoSelect },
	{ "font" , &_lua_button::getFont , &_lua_button::setFont },
	{ "fontSize" , &_lua_button::getFontSize , &_lua_button::setFontSize },
	LUA_CLASS_ATTR_END
};