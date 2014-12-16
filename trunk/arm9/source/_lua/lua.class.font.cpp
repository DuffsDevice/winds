#include <_lua/lua.class.font.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*##################################
##           Lua-Gagget           ##
##################################*/

//! Ctor
_lua_font::_lua_font( _fontHandle f ) :
	font( f )
{}

//! Lua-Ctor
_lua_font::_lua_font( lua_State* L ) :
	font( nullptr )
{}

//! getCharacterWidth
int _lua_font::getCharacterWidth( lua_State* L ){ return push( L , this->font->getCharacterWidth( check<_literal>( L , 1 )[0] ) ); }

//! isCharSupported
int _lua_font::isCharSupported( lua_State* L ){ return push( L , this->font->isCharSupported( check<_literal>( L , 1 )[0] ) ); }

//! getAscent
int _lua_font::getAscent( lua_State* L ){ return push( L , this->font->getAscent( lightcheck<_u8>( L , 1 , 0 ) , lightcheck<_literal>( L , 2 , "a" )[0] ) ); }

//! getLetterSpace
int _lua_font::getLetterSpace( lua_State* L ){ return push( L , this->font->getLetterSpace( lightcheck<_u8>( L , 1 , 0 ) , lightcheck<_literal>( L , 2 , "a" )[0] ) ); }

//! getHeight
int _lua_font::getHeight( lua_State* L ){ return push( L , this->font->getHeight( lightcheck<_u8>( L , 1 , 0 ) , lightcheck<_literal>( L , 2 , "a" )[0] ) ); }


//! Lua-_gadget
const char _lua_font::className[] = "Font";
Lunar<_lua_font>::FunctionType _lua_font::methods[] = {
	{ "getCharacterWidth"		, &_lua_font::getCharacterWidth },
	{ "isCharSupported"			, &_lua_font::isCharSupported },
	{ "getStringWidth"			, wrap( _lua_font , (_length (_font::*)(_literal,_u8)const)&_font::getStringWidth ) },
	{ "isValid"					, wrap( _lua_font , &_font::isValid ) },
	{ "getHeight"				, &_lua_font::getHeight },
	{ "getAscent"				, &_lua_font::getAscent },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_font>::PropertyType _lua_font::properties[] = {
	{ "height"		, &_lua_font::getHeight						, nullptr },
	{ "ascent"		, &_lua_font::getAscent						, nullptr },
	{ "name"		, wrap( _lua_font , &_font::getName )		, nullptr },
	{ "monospace"	, wrap( _lua_font , &_font::isMonospace )	, nullptr },
	LUA_CLASS_ATTR_END
};