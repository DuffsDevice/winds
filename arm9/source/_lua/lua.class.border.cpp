#include <_lua/lua.class.border.h>
#include <_lua/lua.func.h>
using namespace _luafunc;

/*##################################
##           Lua-Border           ##
##################################*/

_lua_border::_lua_border( _border rc ) : _border( rc )
{}

//! Constructor
_lua_border::_lua_border( lua_State* L ) : 
	_border( check<int>( L , 1 ) , check<int>( L , 2 ) , check<int>( L , 3 ) , check<int>( L , 4 ) )
{ }
//! setLeft
int _lua_border::setLeft(lua_State* L){ _border::left = check<int>( L , 1 ); return 0; }

//! setRight
int _lua_border::setRight(lua_State* L){ _border::right = check<int>( L , 1 ); return 0; }

//! setTop
int _lua_border::setTop(lua_State* L){ _border::top = check<int>( L , 1 ); return 0; }

//! setBottom
int _lua_border::setBottom(lua_State* L){ _border::bottom = check<int>( L , 1 ); return 0; }

//! getLeft
int _lua_border::getLeft(lua_State* L){ return push( L , _border::left );  }

//! getTop
int _lua_border::getTop(lua_State* L){ return push( L , _border::top ); }

//! getRight
int _lua_border::getRight(lua_State* L){ return push( L , _border::right ); }

//! getBottom
int _lua_border::getBottom(lua_State* L){ return push( L , _border::bottom ); }

//! Lua-_rect
const char _lua_border::className[] = "Border";
Lunar<_lua_border>::FunctionType _lua_border::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_border>::PropertyType _lua_border::properties[] = {
	{ "left" , &_lua_border::getLeft , &_lua_border::setLeft },
	{ "top" , &_lua_border::getTop , &_lua_border::setTop },
	{ "right" , &_lua_border::getRight , &_lua_border::setRight },
	{ "bottom" , &_lua_border::getBottom , &_lua_border::setBottom },
	LUA_CLASS_ATTR_END
};