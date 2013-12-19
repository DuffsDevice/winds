#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.font.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##           Lua-Bitmap           ##
##################################*/

// Ctor
_lua_bitmap::_lua_bitmap( _bitmap* b ) : 
	bm( b )
	, wasAllocated( false )
{ }

_lua_bitmap::_lua_bitmap( _constbitmap* b ) :
	bm( new _bitmap( *b ) ) // Copy bitmap
	, wasAllocated( true )
{ }

_lua_bitmap::_lua_bitmap( _bitmap&& b ) :
	bm( new _bitmap( move(b) ) ) // Copy bitmap
	, wasAllocated( true )
{ }

// Lua-Ctor
_lua_bitmap::_lua_bitmap( lua_State* L ) : wasAllocated( true ) {
	if( !lua_gettop(L) )
		this->bm = new _bitmap();
	else
		this->bm = new _bitmap( check<int>( L , 1 ) , check<int>( L , 2 ) , lightcheck<_pixel>( L , 3 , NO_COLOR ) );
}

//! Dtor
_lua_bitmap::~_lua_bitmap(){ if( this->bm != nullptr && this->wasAllocated ) delete this->bm; }

//! opertor[pos] and operator(x,y)
int _lua_bitmap::get( lua_State* L ){
	if( lua_gettop(L) == 1 )
		lua_pushnumber( L , this->bm->operator[]( check<int>( L , 1 ) ) );
	else
		lua_pushnumber( L , this->bm->operator()( check<int>( L , 1 ) , check<int>( L , 2 ) ) );
	return 1;
}

using shortCutType = void (_bitmap::*)(_coord,_coord,const _font*,const _char*,_pixel,_u8);

//! Lua-_gadget
const char _lua_bitmap::className[] = "Bitmap";
Lunar<_lua_bitmap>::FunctionType _lua_bitmap::methods[] = {
	{ "get"					, &_lua_bitmap::get },
	{ "resize"				, wrap( _lua_bitmap , &_bitmap::resize ) },
	{ "getPixel"			, wrap( _lua_bitmap , &_bitmap::getPixel ) },
	{ "drawPixel"			, wrap( _lua_bitmap , &_bitmap::drawPixel ) },
	{ "reset"				, wrap( _lua_bitmap , &_bitmap::reset ) },
	{ "fill"				, wrap( _lua_bitmap , &_bitmap::fill ) },
	{ "replaceColor"		, wrap( _lua_bitmap , &_bitmap::replaceColor ) },
	{ "drawLine"			, wrap( _lua_bitmap , &_bitmap::drawLine ) },
	{ "drawVerticalLine"	, wrap( _lua_bitmap , &_bitmap::drawVerticalLine ) },
	{ "drawHorizontalLine"	, wrap( _lua_bitmap , &_bitmap::drawHorizontalLine ) },
	{ "drawVerticalDottedLine"		, wrap( _lua_bitmap , &_bitmap::drawVerticalDottedLine ) },
	{ "drawHorizontalDottedLine"	, wrap( _lua_bitmap , &_bitmap::drawHorizontalDottedLine ) },
	{ "drawRect"			, wrap( _lua_bitmap , &_bitmap::drawRect ) },
	{ "drawDottedRect"		, wrap( _lua_bitmap , &_bitmap::drawDottedRect ) },
	{ "drawFilledRect"		, wrap( _lua_bitmap , &_bitmap::drawFilledRect ) },
	{ "drawVerticalGradient"		, wrap( _lua_bitmap , &_bitmap::drawVerticalGradient ) },
	{ "drawHorizontalGradient"		, wrap( _lua_bitmap , &_bitmap::drawHorizontalGradient ) },
	{ "drawCircle"			, wrap( _lua_bitmap , &_bitmap::drawCircle ) },
	{ "drawFilledCircle"	, wrap( _lua_bitmap , &_bitmap::drawFilledCircle ) },
	{ "drawEllipse"			, wrap( _lua_bitmap , &_bitmap::drawEllipse ) },
	{ "drawFilledEllipse"	, wrap( _lua_bitmap , &_bitmap::drawFilledEllipse ) },
	{ "drawChar"			, wrap( _lua_bitmap , &_bitmap::drawChar ) },
	{ "drawString"			, wrap( _lua_bitmap , (shortCutType)&_bitmap::drawString ) },
	{ "copy"				, wrap( _lua_bitmap , &_bitmap::copy ) },
	{ "copyTransparent"		, wrap( _lua_bitmap , &_bitmap::copyTransparent ) },
	{ "copyHorizontalStretch"		, wrap( _lua_bitmap , &_bitmap::copyHorizontalStretch ) },
	{ "getClippingRect"		, wrap( _lua_bitmap , &_bitmap::getClippingRect ) },
	{ "setClippingRect"		, wrap( _lua_bitmap , &_bitmap::setClippingRect ) },
	{ "resetClippingRect"	, wrap( _lua_bitmap , &_bitmap::resetClippingRect ) },
	{ "isValid"				, wrap( _lua_bitmap , &_bitmap::isValid ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_bitmap>::PropertyType _lua_bitmap::properties[] = {
	{ "width"	, wrap( _lua_bitmap , &_bitmap::getWidth )	, wrap( _lua_bitmap , &_bitmap::setWidth ) },
	{ "height"	, wrap( _lua_bitmap , &_bitmap::getHeight )	, wrap( _lua_bitmap , &_bitmap::setHeight ) },
	LUA_CLASS_ATTR_END
};