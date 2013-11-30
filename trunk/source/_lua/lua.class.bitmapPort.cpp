#include "_lua/lua.class.bitmapPort.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.area.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##         Lua-Bitmap-Port        ##
##################################*/

// Ctor
_lua_bitmapPort::_lua_bitmapPort( _bitmapPort b ) :
	_bitmapPort( b )
{}

// Lua-Ctor
_lua_bitmapPort::_lua_bitmapPort( lua_State* L ) :
	_bitmapPort( *Lunar<_lua_bitmap>::check( L , 1 )->bm )
{ }

int _lua_bitmapPort::addClippingRects( lua_State* L ){
	if( is_a<_rect>( L , 1 ) )
		_bitmapPort::addClippingRects( check<_rect>( L , 1 ) );
	else
		_bitmapPort::addClippingRects( check<_area>( L , 1 ) );
	return 0;
}

using shortCutType = void (_bitmapPort::*)(_coord,_coord,const _font*,const _char*,_pixel,_u8);

//! Lua-_gadget
const char _lua_bitmapPort::className[] = "BitmapPort";
Lunar<_lua_bitmapPort>::FunctionType _lua_bitmapPort::methods[] = {
	{ "addClippingRects"	, &_lua_bitmapPort::addClippingRects },
	{ "normalizeClippingRects"		, wrap( _lua_bitmapPort , &_bitmapPort::normalizeClippingRects ) },
	{ "deleteClippingRects"	, wrap( _lua_bitmapPort , &_bitmapPort::deleteClippingRects ) },
	{ "getPixel"			, wrap( _lua_bitmapPort , &_bitmapPort::getPixel ) },
	{ "drawPixel"			, wrap( _lua_bitmapPort , &_bitmapPort::drawPixel ) },
	{ "fill"				, wrap( _lua_bitmapPort , &_bitmapPort::fill ) },
	{ "replaceColor"		, wrap( _lua_bitmapPort , &_bitmapPort::replaceColor ) },
	{ "drawLine"			, wrap( _lua_bitmapPort , &_bitmapPort::drawLine ) },
	{ "drawVerticalLine"	, wrap( _lua_bitmapPort , &_bitmapPort::drawVerticalLine ) },
	{ "drawHorizontalLine"	, wrap( _lua_bitmapPort , &_bitmapPort::drawHorizontalLine ) },
	{ "drawVerticalDottedLine"		, wrap( _lua_bitmapPort , &_bitmapPort::drawVerticalDottedLine ) },
	{ "drawHorizontalDottedLine"	, wrap( _lua_bitmapPort , &_bitmapPort::drawHorizontalDottedLine ) },
	{ "drawRect"			, wrap( _lua_bitmapPort , &_bitmapPort::drawRect ) },
	{ "drawDottedRect"		, wrap( _lua_bitmapPort , &_bitmapPort::drawDottedRect ) },
	{ "drawFilledRect"		, wrap( _lua_bitmapPort , &_bitmapPort::drawFilledRect ) },
	{ "drawVerticalGradient"		, wrap( _lua_bitmapPort , &_bitmapPort::drawVerticalGradient ) },
	{ "drawHorizontalGradient"		, wrap( _lua_bitmapPort , &_bitmapPort::drawHorizontalGradient ) },
	{ "drawCircle"			, wrap( _lua_bitmapPort , &_bitmapPort::drawCircle ) },
	{ "drawFilledCircle"	, wrap( _lua_bitmapPort , &_bitmapPort::drawFilledCircle ) },
	{ "drawEllipse"			, wrap( _lua_bitmapPort , &_bitmapPort::drawEllipse ) },
	{ "drawFilledEllipse"	, wrap( _lua_bitmapPort , &_bitmapPort::drawFilledEllipse ) },
	{ "drawChar"			, wrap( _lua_bitmapPort , &_bitmapPort::drawChar ) },
	{ "drawString"			, wrap( _lua_bitmapPort , (shortCutType)&_bitmapPort::drawString ) },
	{ "copy"				, wrap( _lua_bitmapPort , &_bitmapPort::copy ) },
	{ "copyTransparent"		, wrap( _lua_bitmapPort , &_bitmapPort::copyTransparent ) },
	{ "copyHorizontalStretch"		, wrap( _lua_bitmapPort , &_bitmapPort::copyHorizontalStretch ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_bitmapPort>::PropertyType _lua_bitmapPort::properties[] = {
	{ "width"	, wrap( _lua_bitmapPort , &_bitmapPort::getWidth )	, nullptr },
	{ "height"	, wrap( _lua_bitmapPort , &_bitmapPort::getHeight )	, nullptr },
	LUA_CLASS_ATTR_END
};