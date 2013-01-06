#include "_lua/lua.gadget.label.h"
//#include "_lua/lua.class.font.h"

/*##################################
##            Lua-Label           ##
##################################*/

_lua_label::_lua_label( lua_State* L ) : 
	_lua_gadget( 
		new _label( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkstring( L , 3 ) )
	)
	, _lua_interface_input( (_label*)_lua_gadget::gadget )
{ }

int _lua_label::getColor( lua_State* L ){
	lua_pushnumber( L , ((_label*)this->gadget)->getColor() ); return 1;
}

int _lua_label::setColor( lua_State* L ){
	((_label*)this->gadget)->setColor( luaL_checkint( L , 1 ) ); return 0;
}

int _lua_label::setBgColor( lua_State* L ){
	((_label*)this->gadget)->setColor( luaL_checkint( L , 1 ) ); return 0;
}

int _lua_label::getBgColor( lua_State* L ){
	lua_pushnumber( L , ((_label*)this->gadget)->getBgColor() ); return 1;
}

int _lua_label::setFont( lua_State* L ){
	/*_font* f = Lunar<_lua_font>::check( L , 1 )->font;
	if( f )
		((_label*)this->gadget)->setFont( f );
	return 0;*/
	//! TODO: implement
	return 0;
}

int _lua_label::getFont( lua_State* L ){
	//Lunar<_lua_font>::push( L , new _lua_font( ((_label*)this->gadget)->getFont() ) , true ); return 1;
	//!TODO: implement
	return 0;
}

//! Lua-window
const char _lua_label::className[] = "_label";
Lunar<_lua_label>::FunctionType _lua_label::methods[] = {
	GADGET_FUNCS( _lua_label ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_label>::PropertyType _lua_label::properties[] = {
	GADGET_ATTRS( _lua_label ),
	//! _interface_input (only these two are used)
	{ "strValue" , &_lua_label::getStrValue , &_lua_label::setStrValue },
	{ "color" , &_lua_label::getColor , &_lua_label::setColor },
	{ "bgColor" , &_lua_label::getBgColor , &_lua_label::setBgColor },
	{ "font" , &_lua_label::getFont , &_lua_label::setFont },
	LUA_CLASS_ATTR_END
};