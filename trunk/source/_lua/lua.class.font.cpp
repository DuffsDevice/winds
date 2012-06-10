#include "_lua/lua.class.font.h"

/*##################################
##           Lua-Gagget           ##
##################################*/

//! Ctor
_lua_font::_lua_font( _freetypefont* f ) :
	font( f )
{ }

//! Lua-Ctor
_lua_font::_lua_font( lua_State* L ) : 
	font( new _freetypefont( luaL_checkstring( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) ) )
{ }

//! getStringWidth
int _lua_font::getStringWidth( lua_State* L ){ lua_pushnumber( L , this->font->getStringWidth( luaL_checkstring( L , 1 ) ) ); return 1; }


//! Lua-_gadget
const char _lua_font::className[] = "_freetypefont";
Lunar<_lua_font>::RegType _lua_font::methods[] = {
  LUNAR_DECLARE_METHOD( _lua_font , getStringWidth ),
  {0,0}
};