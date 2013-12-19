#include "_lua/lua.class.color.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##            Lua-Color           ##
##################################*/

_lua_color::_lua_color( _color rc ) : _color( rc )
{}

//! Constructor
_lua_color::_lua_color( lua_State* L )
{
	if( lua_gettop( L ) == 1 )
		_color::setColor( check<_pixel>( L , 1 ) );
}

//! setR
int _lua_color::setR(lua_State* L){ _color::setR( check<int>( L , 1 ) ); return 0; }

//! setG
int _lua_color::setG(lua_State* L){ _color::setG( check<int>( L , 1 ) ); return 0; }

//! setB
int _lua_color::setB(lua_State* L){ _color::setB( check<int>( L , 1 ) ); return 0; }

//! setH
int _lua_color::setH(lua_State* L){ _color::setH( check<int>( L , 1 ) ); return 0; }

//! setS
int _lua_color::setS(lua_State* L){ _color::setS( check<int>( L , 1 ) ); return 0; }

//! setL
int _lua_color::setL(lua_State* L){ _color::setL( check<int>( L , 1 ) ); return 0; }

//! setColor
int _lua_color::setColor(lua_State* L){ _color::setColor( check<_pixel>( L , 1 ) ); return 0; }

//! setRGB
int _lua_color::setRGB(lua_State* L){ _color::setRGB( check<int>( L , 1 ) , lightcheck<int>( L , 2 , 100 ) , lightcheck<int>( L , 3 , 50 ) ); return 0; }

//! setHSL
int _lua_color::setHSL(lua_State* L){ _color::setHSL( check<int>( L , 1 ) , lightcheck<int>( L , 2 , 100 ) , lightcheck<int>( L , 3 , 50 ) ); return 0; }

//! adjustRGB
int _lua_color::adjustRGB(lua_State* L){ _color::adjustRGB( check<int>( L , 1 ) , lightcheck<int>( L , 2 , 0 ) , lightcheck<int>( L , 3 , 0 ) ); return 0; }

//! adjustHSL
int _lua_color::adjustHSL(lua_State* L){ _color::adjustHSL( check<int>( L , 1 ) , lightcheck<int>( L , 2 , 0 ) , lightcheck<int>( L , 3 , 0 ) ); return 0; }

//! Lua-_rect
const char _lua_color::className[] = "Color";
Lunar<_lua_color>::FunctionType _lua_color::methods[] = {
	{ "setRGB" , &_lua_color::setRGB },
	{ "setHSL" , &_lua_color::setHSL },
	{ "adjustRGB" , &_lua_color::adjustRGB },
	{ "adjustHSL" , &_lua_color::adjustHSL },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_color>::PropertyType _lua_color::properties[] = {
	{ "red" , wrap( _lua_color , &_color::getR ) , &_lua_color::setR },
	{ "green" , wrap( _lua_color , &_color::getG ) , &_lua_color::setG },
	{ "blue" , wrap( _lua_color , &_color::getB ) , &_lua_color::setB },
	{ "hue" , wrap( _lua_color , &_color::getH ) ,&_lua_color::setH },
	{ "sat" , wrap( _lua_color , &_color::getS ) , &_lua_color::setS },
	{ "lum" , wrap( _lua_color , &_color::getL ) , &_lua_color::setL },
	{ "color" , wrap( _lua_color , &_color::getColor ) , &_lua_color::setColor },
	LUA_CLASS_ATTR_END
};