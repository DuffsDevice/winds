#include "_lua/lua.class.font.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##           Lua-Gagget           ##
##################################*/

//! Ctor
_lua_font::_lua_font( _fontPtr f ) :
	font( f )
{}

//! Lua-Ctor
_lua_font::_lua_font( lua_State* L ) : 
	font( _font::fromFile( check<string>( L , 1 ) ) )
{}

//! getCharacterWidth
int _lua_font::getCharacterWidth( lua_State* L ){ lua_pushnumber( L , this->font->getCharacterWidth( check<string>( L , 1 )[0] ) ); return 1; }

//! isCharSupported
int _lua_font::isCharSupported( lua_State* L ){ lua_pushnumber( L , this->font->isCharSupported( check<string>( L , 1 )[0] ) ); return 1; }


//! Lua-_gadget
const char _lua_font::className[] = "Font";
Lunar<_lua_font>::FunctionType _lua_font::methods[] = {
	{ "getCharacterWidth"		, &_lua_font::getCharacterWidth },
	{ "isCharSupported"			, &_lua_font::isCharSupported },
	{ "getStringWidth"			, wrap( _lua_font , (_length (_font::*)(_literal,_u8)const)&_font::getStringWidth ) },
	{ "isValid"					, wrap( _lua_font , &_font::isValid ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_font>::PropertyType _lua_font::properties[] = {
	{ "height"		, wrap( _lua_font , &_font::getHeight )		, nullptr },
	{ "ascent"		, wrap( _lua_font , &_font::getAscent )		, nullptr },
	{ "name"		, wrap( _lua_font , &_font::getName )		, nullptr },
	{ "monospace"	, wrap( _lua_font , &_font::isMonospace )	, nullptr },
	LUA_CLASS_ATTR_END
};