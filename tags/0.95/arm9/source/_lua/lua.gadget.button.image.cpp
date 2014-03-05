#include "_lua/lua.gadget.button.image.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##           Lua-Button           ##
##################################*/

_lua_imagebutton::_lua_imagebutton( lua_State* L ) :
	_lua_gadget( new _imageButton( optcheck<int>( L , 1 ) , optcheck<int>( L , 2 ) , optcheck<int>( L , 3 ) , optcheck<int>( L , 4 ) , check<_bitmap>( L , 5 ) , check<_bitmap>( L , 6 ) , lightcheck<string>( L , 7 , "" ) , lightcheck<_style>( L , 8 ) ) )
{}

//! Lua-button
const char _lua_imagebutton::className[] = "ImageButton";
Lunar<_lua_imagebutton>::FunctionType _lua_imagebutton::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_imagebutton>::PropertyType _lua_imagebutton::properties[] = {
	{ "image"			, wrap( _lua_imagebutton , &_imageButton::getImage )		, wrap( _lua_imagebutton , &_imageButton::setImage ) },
	{ "imagePressed"	, wrap( _lua_imagebutton , &_imageButton::getImagePressed )	, wrap( _lua_imagebutton , &_imageButton::setImagePressed ) },
	LUA_CLASS_ATTR_END
};