#include "_lua/lua.gadget.imagegadget.h"

/*##################################
##           Lua-Window           ##
##################################*/

_lua_imagegadget::_lua_imagegadget( lua_State* L ) : 
	_lua_gadget( new _imagegadget( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , Lunar<_lua_bitmap>::check( L , 3 )->bm ) )
{ }

int _lua_imagegadget::setImage( lua_State* L ){
	((_imagegadget*)_lua_gadget::gadget)->setImage( Lunar<_lua_bitmap>::check( L , 1 )->bm );
	return 0;
}

int _lua_imagegadget::getImage( lua_State* L ){
	Lunar<_lua_bitmap>::push( L , new _lua_bitmap( const_cast<_bitmap*>( ((_imagegadget*)_lua_gadget::gadget)->getImage() ) ) , true );
	return 1;
}

//! Lua-window
const char _lua_imagegadget::className[] = "_imagegadget";

Lunar<_lua_imagegadget>::RegType _lua_imagegadget::methods[] = {
	LUNAR_DECLARE_ALL_METHODS_OF( _lua_imagegadget ),
	
	//! Specific to _imagegadget
	LUNAR_DECLARE_METHOD(_lua_imagegadget,getImage),
	LUNAR_DECLARE_METHOD(_lua_imagegadget,setImage),
	{0,0}
};