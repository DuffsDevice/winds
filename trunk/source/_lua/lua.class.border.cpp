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
int _lua_border::setLeft(lua_State* L){ _border::setLeft( luaL_checkint( L , 1 ) ); return 0; }

//! setRight
int _lua_border::setRight(lua_State* L){ _border::setRight( luaL_checkint( L , 1 ) ); return 0; }

//! setTop
int _lua_border::setTop(lua_State* L){ _border::setTop( luaL_checkint( L , 1 ) ); return 0; }

//! setBottom
int _lua_border::setBottom(lua_State* L){ _border::setBottom( luaL_checkint( L , 1 ) ); return 0; }

//! getLeft
int _lua_border::getLeft(lua_State* L){ lua_pushnumber( L , _border::getLeft() ); return 1; }

//! getTop
int _lua_border::getTop(lua_State* L){ lua_pushnumber( L , _border::getTop() ); return 1; }

//! getRight
int _lua_border::getRight(lua_State* L){ lua_pushnumber( L , _border::getRight() ); return 1; }

//! getBottom
int _lua_border::getBottom(lua_State* L){ lua_pushnumber( L , _border::getBottom() ); return 1; }

//! Lua-_rect
const char _lua_border::className[] = "_border";
Lunar<_lua_border>::RegType _lua_border::methods[] = {
  LUNAR_DECLARE_METHOD(_lua_border, setLeft),
  LUNAR_DECLARE_METHOD(_lua_border, setTop),
  LUNAR_DECLARE_METHOD(_lua_border, setRight),
  LUNAR_DECLARE_METHOD(_lua_border, setBottom),
  LUNAR_DECLARE_METHOD(_lua_border, getLeft),
  LUNAR_DECLARE_METHOD(_lua_border, getTop),
  LUNAR_DECLARE_METHOD(_lua_border, getRight),
  LUNAR_DECLARE_METHOD(_lua_border, getBottom),
  {0,0}
};