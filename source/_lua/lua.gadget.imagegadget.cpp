#include "_lua/lua.gadget.imagegadget.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##           Lua-Window           ##
##################################*/

_lua_imagegadget::_lua_imagegadget( lua_State* L ) : 
	_lua_gadget( new _imagegadget( check<int>( L , 1 ) , check<int>( L , 2 ) , *Lunar<_lua_bitmap>::check( L , 3 )->bm , lightcheck<_style>( L , 4 , _style() ) ) )
{ }

int _lua_imagegadget::setImage( lua_State* L ){
	_lua_bitmap* bm = Lunar<_lua_bitmap>::check( L , 1 );
	if( bm && bm->bm )
		((_imagegadget*)_lua_gadget::getGadget())->setImage( *bm->bm );
	return 0;
}

int _lua_imagegadget::getImage( lua_State* L ){
	Lunar<_lua_bitmap>::push( L , new _lua_bitmap( &((_imagegadget*)_lua_gadget::getGadget())->getImage() ) );
	return 1;
}

//! Lua-window
const char _lua_imagegadget::className[] = "ImageGadget";
Lunar<_lua_imagegadget>::FunctionType _lua_imagegadget::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_imagegadget>::PropertyType _lua_imagegadget::properties[] = {
	GADGET_BASE_ATTR,
	//! _interface_input (only these two are used)
	{ "image" , &_lua_imagegadget::getImage , &_lua_imagegadget::setImage },
	LUA_CLASS_ATTR_END
};