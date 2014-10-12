#include <_lua/lua.class.imagefile.h>
#include <_lua/lua.class.bitmap.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*##################################
##          Lua-Imagefile         ##
##################################*/

_lua_imagefile::_lua_imagefile( lua_State* L ) : 
	_imageFile( check<string>( L , 1 ) , lightcheck<bool>( L , 2 , false ) )
{ }

//! Lua-ImageFile
const char _lua_imagefile::className[] = "ImageFile";
Lunar<_lua_imagefile>::FunctionType _lua_imagefile::methods[] = {
	{ "readBitmap" , wrap( _lua_imagefile , &_imageFile::readBitmap ) },
	{ "writeBitmap" , wrap( _lua_imagefile , &_imageFile::writeBitmap ) },
	{ "supportsPages" , wrap( _lua_imagefile , &_imageFile::supportsPages ) },
	{ "getPageDelay" , wrap( _lua_imagefile , &_imageFile::getPageDelay ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_imagefile>::PropertyType _lua_imagefile::properties[] = {
	{ "numPages" , wrap( _lua_imagefile , &_imageFile::getNumPages ) , nullptr },
	LUA_CLASS_ATTR_END
};