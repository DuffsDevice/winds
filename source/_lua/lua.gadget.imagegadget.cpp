#include "_lua/lua.gadget.imagegadget.h"

/*##################################
##           Lua-Window           ##
##################################*/

_lua_imagegadget::_lua_imagegadget( lua_State* L ) : 
	_lua_gadget( new _imagegadget( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , *Lunar<_lua_bitmap>::check( L , 3 )->bm ) )
{ }

int _lua_imagegadget::setImage( lua_State* L ){
	((_imagegadget*)_lua_gadget::gadget)->setImage( *Lunar<_lua_bitmap>::check( L , 1 )->bm );
	return 0;
}

int _lua_imagegadget::getImage( lua_State* L ){
	Lunar<_lua_bitmap>::push( L , new _lua_bitmap( const_cast<_bitmap*>( &((_imagegadget*)_lua_gadget::gadget)->getImage() ) ) );
	return 1;
}

//! Lua-window
const char _lua_imagegadget::className[] = "_imagegadget";
Lunar<_lua_imagegadget>::FunctionType _lua_imagegadget::methods[] = {
	GADGET_FUNCS( _lua_imagegadget ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_imagegadget>::PropertyType _lua_imagegadget::properties[] = {
	GADGET_ATTRS( _lua_imagegadget ),
	//! _interface_input (only these two are used)
	{ "image" , &_lua_imagegadget::getImage , &_lua_imagegadget::setImage },
	LUA_CLASS_ATTR_END
};