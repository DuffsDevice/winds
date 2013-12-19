#include "_lua/lua.class.imagefile.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.func.h"
using namespace _luafunc;

/*##################################
##          Lua-Imagefile         ##
##################################*/

_lua_imagefile::_lua_imagefile( lua_State* L ) : 
	_imagefile( check<string>( L , 1 ) )
{ }

int _lua_imagefile::getBitmap( lua_State* L ){
	if( !_imagefile::isValid() )
		return 0;
	push( L , (_constbitmap&)(_imagefile&)*this );
	return 1;
}

//! Lua-ImageFile
const char _lua_imagefile::className[] = "ImageFile";
Lunar<_lua_imagefile>::FunctionType _lua_imagefile::methods[] = {
	{ "getBitmap" , &_lua_imagefile::getBitmap },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_imagefile>::PropertyType _lua_imagefile::properties[] = {
	LUA_CLASS_ATTR_END
};