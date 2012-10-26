#include "_lua/lua.gadget.textbox.h"
//#include "_lua/lua.class.font.h"

/*##################################
##            Lua-Label           ##
##################################*/

_lua_textbox::_lua_textbox( lua_State* L ) : 
	_lua_gadget( 
		new _textbox( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkstring( L , 4 ) )
	)
	, _lua_interface_input( (_textbox*)_lua_gadget::gadget )
{ }

int _lua_textbox::getColor( lua_State* L ){
	lua_pushnumber( L , ((_textbox*)this->gadget)->getColor() ); return 1;
}

int _lua_textbox::setColor( lua_State* L ){
	((_textbox*)this->gadget)->setColor( luaL_checkint( L , 1 ) ); return 0;
}

int _lua_textbox::setBgColor( lua_State* L ){
	((_textbox*)this->gadget)->setColor( luaL_checkint( L , 1 ) ); return 0;
}

int _lua_textbox::getBgColor( lua_State* L ){
	lua_pushnumber( L , ((_textbox*)this->gadget)->getBgColor() ); return 1;
}

int _lua_textbox::setFont( lua_State* L ){
	/*_font* f = Lunar<_lua_font>::check( L , 1 )->font;
	if( f )
		((_textbox*)this->gadget)->setFont( f );
	return 0;*/
	//! TODO: implement
	return 0;
}

int _lua_textbox::getFont( lua_State* L ){
	//Lunar<_lua_font>::push( L , new _lua_font( ((_textbox*)this->gadget)->getFont() ) , true ); return 1;
	return 0;
	//!TODO: implement
}

//! Lua-window
const char _lua_textbox::className[] = "_textbox";

Lunar<_lua_textbox>::RegType _lua_textbox::methods[] = {
	LUNAR_DECLARE_ALL_METHODS_OF( _lua_textbox ),
	LUNAR_DECLARE_METHOD(_lua_textbox,getColor),
	LUNAR_DECLARE_METHOD(_lua_textbox,setColor),
	LUNAR_DECLARE_METHOD(_lua_textbox,getBgColor),
	LUNAR_DECLARE_METHOD(_lua_textbox,getFont),
	LUNAR_DECLARE_METHOD(_lua_textbox,setBgColor),
	LUNAR_DECLARE_METHOD(_lua_textbox,setFont),
	
	//! _interface_input (only these two are used)
	LUNAR_DECLARE_METHOD(_lua_textbox,getStrValue),
	LUNAR_DECLARE_METHOD(_lua_textbox,setStrValue),
	{0,0}
};