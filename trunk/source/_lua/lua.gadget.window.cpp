#include "_lua/lua.gadget.window.h"
#include "_lua/lua.class.bitmap.h"

/*##################################
##           Lua-Window           ##
##################################*/

_lua_window::_lua_window( lua_State* L ) : 
	_lua_gadget( (_window*)nullptr )
	, _lua_interface_input( nullptr )
{
	if( lua_gettop( L ) == 5 || lua_isstring( L , 6 ) )
	{
		this->setGadget(
			new _window(
			luaL_checkint( L , 1 )
				, luaL_checkint( L , 2 )
				, luaL_checkint( L , 3 )
				, luaL_checkint( L , 4 )
				, luaL_checkstring( L , 5 )
				, luaL_optstyle( L , 6 ) 
			)
		);
	}
	else
	{
		_lua_bitmap* bmp = Lunar<_lua_bitmap>::check( L , 6 );
		
		if( !bmp || !bmp->bm )
			return;
		
		this->setGadget( 
			new _window(
				luaL_checkint( L , 1 )
				, luaL_checkint( L , 2 )
				, luaL_checkint( L , 3 )
				, luaL_checkint( L , 4 )
				, luaL_checkstring( L , 5 )
				, *bmp->bm
				, luaL_optstyle( L , 7 )
			) 
		);
	}
	
	_lua_interface_input::input = (_window*)_lua_gadget::gadget;
}

//! setIcon
int _lua_window::setIcon( lua_State* L )
{
	_lua_bitmap* bmp = Lunar<_lua_bitmap>::check( L , 1 ); 
	if( !bmp )
		return 0;
	((_window*)_lua_gadget::gadget)->setIcon( *(bmp->bm) );
	return 0;
}

//! hasIcon
int _lua_window::hasIcon( lua_State* L )
{
	lua_pushnumber( L , ((_window*)_lua_gadget::gadget)->hasIcon() );
	return 1;
}

//! getIcon
int _lua_window::getIcon( lua_State* L )
{
	Lunar<_lua_bitmap>::push( L , new _lua_bitmap( ((_window*)_lua_gadget::gadget)->getIcon() ) );
	return 1;
}

//! Lua-window
const char _lua_window::className[] = "_window";
Lunar<_lua_window>::FunctionType _lua_window::methods[] = {
	GADGET_FUNCS( _lua_window ),
	LUA_CLASS_FUNC( _lua_window , hasIcon ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_window>::PropertyType _lua_window::properties[] = {
	GADGET_ATTRS( _lua_window ),
	{ "strValue" , &_lua_window::getStrValue , &_lua_window::setStrValue },
	{ "icon" , &_lua_window::getIcon , &_lua_window::setIcon },
	LUA_CLASS_ATTR_END
};