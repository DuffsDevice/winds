#include "_lua/lua.class.border.h"


/*##################################
##           Lua-Border           ##
##################################*/

_lua_border::_lua_border( _border rc ) : _border( rc )
{}

//! Constructor
_lua_border::_lua_border( lua_State* L ) : 
	_border( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) )
{ }
//! setLeft
int _lua_border::setLeft(lua_State* L){ _border::left = luaL_checkint( L , 1 ); return 0; }

//! setRight
int _lua_border::setRight(lua_State* L){ _border::right = luaL_checkint( L , 1 ); return 0; }

//! setTop
int _lua_border::setTop(lua_State* L){ _border::top = luaL_checkint( L , 1 ); return 0; }

//! setBottom
int _lua_border::setBottom(lua_State* L){ _border::bottom = luaL_checkint( L , 1 ); return 0; }

//! getLeft
int _lua_border::getLeft(lua_State* L){ lua_pushnumber( L , _border::left ); return 1; }

//! getTop
int _lua_border::getTop(lua_State* L){ lua_pushnumber( L , _border::top ); return 1; }

//! getRight
int _lua_border::getRight(lua_State* L){ lua_pushnumber( L , _border::right ); return 1; }

//! getBottom
int _lua_border::getBottom(lua_State* L){ lua_pushnumber( L , _border::bottom ); return 1; }

//! Lua-_rect
const char _lua_border::className[] = "_border";
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