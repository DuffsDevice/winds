#include "_lua/lua.class.font.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##           Lua-Gagget           ##
##################################*/

//! Ctor
_lua_font::_lua_font( const _font* f ) :
	font( f )
{ }

//! Lua-Ctor
_lua_font::_lua_font( lua_State* L ) : 
	font( _font::fromFile( check<string>( L , 1 ) ) )
{ }

//! getStringWidth
int _lua_font::getStringWidth( lua_State* L ){ lua_pushnumber( L , this->font->getStringWidth( check<string>( L , 1 ) ) ); return 1; }

//! getCharacterWidth
int _lua_font::getCharacterWidth( lua_State* L ){ lua_pushnumber( L , this->font->getCharacterWidth( check<string>( L , 1 )[0] ) ); return 1; }

//! getNumCharsUntilWidth
int _lua_font::getNumCharsUntilWidth( lua_State* L ){ lua_pushnumber( L , this->font->getNumCharsUntilWidth( check<int>( L , 1 ) , check<string>( L , 2 ) , lightcheck( L , 3 , 0 ) ) ); return 1; }

//! isMonospace
int _lua_font::isMonospace( lua_State* L ){ lua_pushboolean( L , this->font->isMonospace() ); return 1; }

//! isCharSupported
int _lua_font::isCharSupported( lua_State* L ){ lua_pushnumber( L , this->font->isCharSupported( check<string>( L , 1 )[0] ) ); return 1; }

//! getHeight
int _lua_font::getHeight( lua_State* L ){ lua_pushnumber( L , this->font->getHeight() ); return 1; }

//! getAscent
int _lua_font::getAscent( lua_State* L ){ lua_pushnumber( L , this->font->getAscent() ); return 1; }

//! getName
int _lua_font::getName( lua_State* L ){ lua_pushstring( L , this->font->getName().c_str() ); return 1; }

//! isValid
int _lua_font::isValid( lua_State* L ){ lua_pushboolean( L , this->font->isValid() ); return 1; }


//! Lua-_gadget
const char _lua_font::className[] = "Font";
Lunar<_lua_font>::FunctionType _lua_font::methods[] = {
  LUA_CLASS_FUNC( _lua_font , getStringWidth ),
  LUA_CLASS_FUNC( _lua_font , getCharacterWidth ),
  LUA_CLASS_FUNC( _lua_font , getNumCharsUntilWidth ),
  LUA_CLASS_FUNC( _lua_font , isCharSupported ),
  LUA_CLASS_FUNC( _lua_font , isValid ),
  {0,0}
};

Lunar<_lua_font>::PropertyType _lua_font::properties[] = {
	{ "height" , &_lua_font::getHeight , nullptr },
	{ "ascent" , &_lua_font::getAscent , nullptr },
	{ "name" , &_lua_font::getName , nullptr },
	{ "monospace" , &_lua_font::isMonospace , nullptr },
	LUA_CLASS_ATTR_END
};