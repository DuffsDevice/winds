#include "_lua/lua.gadget.imagegadget.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##           Lua-Window           ##
##################################*/

_lua_imagegadget::_lua_imagegadget( lua_State* L ) : 
	_lua_gadget( new _imagegadget( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , check<_bitmap>( L , 3 ) , lightcheck<_style>( L , 4 , _style() ) ) )
{ }

//! Lua-window
const char _lua_imagegadget::className[] = "ImageGadget";
Lunar<_lua_imagegadget>::FunctionType _lua_imagegadget::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_imagegadget>::PropertyType _lua_imagegadget::properties[] = {
	GADGET_BASE_ATTR,
	{ "image" , wrap( _lua_imagegadget , &_imagegadget::getImage ) , wrap( _lua_imagegadget , (void (_imagegadget::*)(_constbitmap&))&_imagegadget::setImage ) },
	LUA_CLASS_ATTR_END
};