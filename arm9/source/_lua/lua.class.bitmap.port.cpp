#include <_lua/lua.class.bitmap.port.h>
#include <_lua/lua.class.bitmap.h>
#include <_lua/lua.class.rect.h>
#include <_lua/lua.class.area.h>
#include <_lua/lua.func.h>
#include <_lua/lua.func.wrap.h>
using namespace _luafunc;

/*##################################
##         Lua-Bitmap-Port        ##
##################################*/

// Ctor
_lua_bitmapport::_lua_bitmapport( _bitmapPort b ) :
	_bitmapPort( b )
{}

// Lua-Ctor
_lua_bitmapport::_lua_bitmapport( lua_State* L ) :
	_bitmapPort( *Lunar<_lua_bitmap>::check( L , 1 )->bm )
{ }

int _lua_bitmapport::addClippingRects( lua_State* L ){
	if( is_a<_rect>( L , 1 ) )
		_bitmapPort::addClippingRects( check<_rect>( L , 1 ) );
	else
		_bitmapPort::addClippingRects( check<_area>( L , 1 ) );
	return 0;
}

using shortCutType = void (_bitmapPort::*)(_coord,_coord,_fontHandle,_literal,_color,_u8);

//! Lua-_gadget
const char _lua_bitmapport::className[] = "BitmapPort";
Lunar<_lua_bitmapport>::FunctionType _lua_bitmapport::methods[] = {
	{ "addClippingRects"			, &_lua_bitmapport::addClippingRects },
	{ "normalizeClippingRects"		, wrap( _lua_bitmapport , &_bitmapPort::normalizeClippingRects ) },
	{ "deleteClippingRects"			, wrap( _lua_bitmapport , &_bitmapPort::deleteClippingRects ) },
	{ "getPixel"					, wrap( _lua_bitmapport , &_bitmapPort::getPixel ) },
	{ "drawPixel"					, wrap( _lua_bitmapport , &_bitmapPort::drawPixel ) },
	{ "fill"						, wrap( _lua_bitmapport , &_bitmapPort::fill ) },
	{ "drawLine"					, wrap( _lua_bitmapport , &_bitmapPort::drawLine ) },
	{ "drawVerticalLine"			, wrap( _lua_bitmapport , &_bitmapPort::drawVerticalLine ) },
	{ "drawHorizontalLine"			, wrap( _lua_bitmapport , &_bitmapPort::drawHorizontalLine ) },
	{ "drawVerticalDottedLine"		, wrap( _lua_bitmapport , &_bitmapPort::drawVerticalDottedLine ) },
	{ "drawHorizontalDottedLine"	, wrap( _lua_bitmapport , &_bitmapPort::drawHorizontalDottedLine ) },
	{ "drawRect"					, wrap( _lua_bitmapport , &_bitmapPort::drawRect ) },
	{ "drawDottedRect"				, wrap( _lua_bitmapport , &_bitmapPort::drawDottedRect ) },
	{ "drawFilledRect"				, wrap( _lua_bitmapport , &_bitmapPort::drawFilledRect ) },
	{ "drawVerticalGradient"		, wrap( _lua_bitmapport , &_bitmapPort::drawVerticalGradient ) },
	{ "drawHorizontalGradient"		, wrap( _lua_bitmapport , &_bitmapPort::drawHorizontalGradient ) },
	{ "drawCircle"					, wrap( _lua_bitmapport , &_bitmapPort::drawCircle ) },
	{ "drawFilledCircle"			, wrap( _lua_bitmapport , &_bitmapPort::drawFilledCircle ) },
	{ "drawEllipse"					, wrap( _lua_bitmapport , &_bitmapPort::drawEllipse ) },
	{ "drawFilledEllipse"			, wrap( _lua_bitmapport , &_bitmapPort::drawFilledEllipse ) },
	{ "drawChar"					, wrap( _lua_bitmapport , &_bitmapPort::drawChar ) },
	{ "drawString"					, wrap( _lua_bitmapport , (shortCutType)&_bitmapPort::drawString ) },
	{ "copy"						, wrap( _lua_bitmapport , &_bitmapPort::copy ) },
	{ "copyTransparent"				, wrap( _lua_bitmapport , &_bitmapPort::copyTransparent ) },
	{ "copyHorizontalStretch"		, wrap( _lua_bitmapport , &_bitmapPort::copyHorizontalStretch ) },
	LUA_CLASS_FUNC_END
};

Lunar<_lua_bitmapport>::PropertyType _lua_bitmapport::properties[] = {
	{ "width"	, wrap( _lua_bitmapport , &_bitmapPort::getWidth )	, nullptr },
	{ "height"	, wrap( _lua_bitmapport , &_bitmapPort::getHeight )	, nullptr },
	LUA_CLASS_ATTR_END
};