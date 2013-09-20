#include "_lua/lua.gadget.textarea.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##          Lua-Textarea          ##
##################################*/

_lua_textarea::_lua_textarea( lua_State* L ) : 
	_lua_gadget( 
		new _textarea( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) , check<int>( L , 4 ) , lightcheck<string>( L , 5 , "" ) , lightcheck<_style>( L , 6 , _style() ) )
	)
	, _lua_interface_input( (_textarea*)_lua_gadget::getGadget() )
{ }


//! setCursor
int _lua_textarea::setCursor( lua_State* L ){ _lua_interface_input::input->setCursor( check<int>( L , 1 ) ); return 0; }

//! getCursor
int _lua_textarea::getCursor( lua_State* L ){ lua_pushnumber( L , _lua_interface_input::input->getCursor() ); return 1; }

//! Lua-window
const char _lua_textarea::className[] = "TextArea";
Lunar<_lua_textarea>::FunctionType _lua_textarea::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_textarea>::PropertyType _lua_textarea::properties[] = {
	GADGET_BASE_ATTR,
	{ "text" , &_lua_textarea::getStrValue , &_lua_textarea::setStrValue },
	{ "color" , &_lua_textarea::getColor , &_lua_textarea::setColor },
	{ "cursor" , &_lua_textarea::getCursor , &_lua_textarea::setCursor },
	{ "bgColor" , &_lua_textarea::getBgColor , &_lua_textarea::setBgColor },
	{ "font" , &_lua_textarea::getFont , &_lua_textarea::setFont },
	{ "fontSize" , &_lua_textarea::getFontSize , &_lua_textarea::setFontSize },
	LUA_CLASS_ATTR_END
};