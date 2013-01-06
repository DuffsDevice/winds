#include "_lua/lua.gadget.button.h"

/*##################################
##           Lua-Button           ##
##################################*/

_lua_button::_lua_button( lua_State* L )
{
	if( lua_gettop( L ) == 3 )
		this->setGadget( new _button( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkstring( L , 3 ) ) );
	else
		this->setGadget( new _button( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkstring( L , 5 ) ) );
	
	_lua_interface_input::input = (_button*)_lua_gadget::gadget;
}

//! setAutoSelect
int _lua_button::setAutoSelect( lua_State* L ){	((_button*)_lua_gadget::gadget)->setAutoSelect( luaL_checkboolean( L , 1 ) ); return 0; };

//! isAutoSelect
int _lua_button::isAutoSelect( lua_State* L ){ lua_pushboolean( L , ((_button*)_lua_gadget::gadget)->isAutoSelect() ); return 1; }

int _lua_button::setFont( lua_State* L ){
	/*_font* f = Lunar<_lua_font>::check( L , 1 )->font;
	if( f )
		((_lua_button*)this->gadget)->setFont( f );
	return 0;*/
	//! TODO: implement
	return 0;
}

int _lua_button::getFont( lua_State* L ){
	//Lunar<_lua_font>::push( L , new _lua_font( ((_lua_button*)this->gadget)->getFont() ) , true ); return 1;
	//!TODO: implement
	return 0;
}

//! Lua-button
const char _lua_button::className[] = "_button";
Lunar<_lua_button>::FunctionType _lua_button::methods[] = {
	GADGET_FUNCS( _lua_button ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_button>::PropertyType _lua_button::properties[] = {
	GADGET_ATTRS( _lua_button ),
	{ "strValue" , &_lua_button::getStrValue , &_lua_button::setStrValue },
	{ "autoSelect" , &_lua_button::isAutoSelect , &_lua_button::setAutoSelect },
	{ "font" , &_lua_button::getFont , &_lua_button::setFont },
	LUA_CLASS_ATTR_END
};