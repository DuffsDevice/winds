#include "_lua/lua.class.bitmapPort.h"
#include "_lua/lua.class.bitmap.h"
#include "_lua/lua.class.rect.h"
#include "_lua/lua.class.area.h"
#include "_lua/lua.class.font.h"

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
	if( luaL_is( L , 1 , "_rect" ) )
		_bitmapPort::addClippingRects( *Lunar<_lua_rect>::check( L , 1 ) );
	else
		_bitmapPort::addClippingRects( *Lunar<_lua_area>::check( L , 1 ) );
	return 0;
}

int _lua_bitmapPort::normalizeClippingRects( lua_State* L ){
	_bitmapPort::normalizeClippingRects();
	return 0;
}

int _lua_bitmapPort::deleteClippingRects( lua_State* L ){
	_bitmapPort::deleteClippingRects();
	return 0;
}

//! getWidth
int _lua_bitmapPort::getWidth(lua_State* L ){ lua_pushnumber( L , _bitmapPort::getWidth() ); return 1; }

//! getHeight
int _lua_bitmapPort::getHeight(lua_State* L ){ lua_pushnumber( L , _bitmapPort::getHeight() ); return 1; }

//! getPixel
int _lua_bitmapPort::getPixel( lua_State* L ){ lua_pushnumber( L , _bitmapPort::getPixel( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) ) ); return 1; }

//! drawPixel
int _lua_bitmapPort::drawPixel( lua_State* L ){ _bitmapPort::drawPixel( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkcolor( L , 3 ) ); return 0; }

//! fill
int _lua_bitmapPort::fill( lua_State* L ){ _bitmapPort::fill( luaL_checkcolor( L , 1 ) ); return 0; }

//! drawVerticalLine
int _lua_bitmapPort::drawVerticalLine( lua_State* L ){ _bitmapPort::drawVerticalLine( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkcolor( L , 4 ) ); return 0; }

//! drawHorizontalLine
int _lua_bitmapPort::drawHorizontalLine( lua_State* L ){ _bitmapPort::drawHorizontalLine( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkcolor( L , 4 ) ); return 0; }

//! drawVerticalDottedLine
int _lua_bitmapPort::drawVerticalDottedLine( lua_State* L ){ _bitmapPort::drawVerticalDottedLine( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkcolor( L , 4 ) ); return 0; }

//! drawHorizontalDottedLine
int _lua_bitmapPort::drawHorizontalDottedLine( lua_State* L ){ _bitmapPort::drawHorizontalDottedLine( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkcolor( L , 4 ) ); return 0; }

//! drawLine
int _lua_bitmapPort::drawLine( lua_State* L ){ _bitmapPort::drawLine( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkcolor( L , 5 ) ); return 0; }

//! replaceColor
int _lua_bitmapPort::replaceColor( lua_State* L ){ _bitmapPort::replaceColor( luaL_checkcolor( L , 1 ) , luaL_checkcolor( L , 2 ) ); return 0; }

//! drawRect
int _lua_bitmapPort::drawRect( lua_State* L ){ _bitmapPort::drawRect( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkcolor( L , 5 ) ); return 0; }

//! drawFilledRect
int _lua_bitmapPort::drawFilledRect( lua_State* L ){ _bitmapPort::drawFilledRect( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkcolor( L , 5 ) ); return 0; }

//! drawVerticalGradient
int _lua_bitmapPort::drawVerticalGradient( lua_State* L ){ 
	_bitmapPort::drawVerticalGradient( 
		luaL_checkint( L , 1 )
		, luaL_checkint( L , 2 )
		, luaL_checkint( L , 3 )
		, luaL_checkint( L , 4 )
		, luaL_checkcolor( L , 5 )
		, luaL_checkcolor( L , 6 )
	); 
	return 0;
}

//! drawHorizontalGradient
int _lua_bitmapPort::drawHorizontalGradient( lua_State* L ){ 
	_bitmapPort::drawHorizontalGradient( 
		luaL_checkint( L , 1 )
		, luaL_checkint( L , 2 )
		, luaL_checkint( L , 3 )
		, luaL_checkint( L , 4 )
		, luaL_checkcolor( L , 5 )
		, luaL_checkcolor( L , 6 )
	); 
	return 0;
}

//! drawCircle
int _lua_bitmapPort::drawCircle( lua_State* L ){ _bitmapPort::drawCircle( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkcolor( L , 4 ) ); return 0; }

//! drawFilledCircle
int _lua_bitmapPort::drawFilledCircle( lua_State* L ){ _bitmapPort::drawFilledCircle( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkcolor( L , 4 ) ); return 0; }

//! drawEllipse
int _lua_bitmapPort::drawEllipse( lua_State* L ){ _bitmapPort::drawEllipse( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkcolor( L , 5 ) ); return 0; }

//! drawFilledEllipse
int _lua_bitmapPort::drawFilledEllipse( lua_State* L ){ _bitmapPort::drawFilledEllipse( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkcolor( L , 5 ) ); return 0; }

//! drawChar
int _lua_bitmapPort::drawChar( lua_State* L )
{ 
	_lua_font* f = Lunar<_lua_font>::check( L , 3 );
	if( !f || !f->font )
		return 0;
	lua_pushnumber( 
		L
		, _bitmapPort::drawChar( 
			luaL_checkint( L , 1 ) ,
			luaL_checkint( L , 2 ) ,
			f->font ,
			luaL_checkstring( L , 4 )[0] ,
			luaL_checkcolor( L , 5 ) 
		)
	);
	return 1;
}
	
//! drawString
int _lua_bitmapPort::drawString( lua_State* L )
{
	_lua_font* f = Lunar<_lua_font>::check( L , 3 );
	if( !f || !f->font )
		return 0;
	_bitmapPort::drawString( 
		luaL_checkint( L , 1 ) ,
		luaL_checkint( L , 2 ) ,
		f->font ,
		luaL_checkstring( L , 4 ) ,
		luaL_checkcolor( L , 5 ) 
	);
	return 0;
}

//! copy
int _lua_bitmapPort::copy( lua_State* L )
{ 
	_lua_bitmap* b = Lunar<_lua_bitmap>::check( L , 3 );
	if( !b )
		return 0;
	_bitmapPort::copy( luaL_checkint( L , 1 ) ,	luaL_checkint( L , 2 ) , *b->bm );
	return 0;
}

//! copyTransparent
int _lua_bitmapPort::copyTransparent( lua_State* L )
{ 
	_lua_bitmap* b = Lunar<_lua_bitmap>::check( L , 3 );
	if( !b )
		return 0;
	_bitmapPort::copyTransparent( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) ,	*b->bm );
	return 0;
}

//! copyHorizontalStretch
int _lua_bitmapPort::copyHorizontalStretch( lua_State* L )
{ 
	_lua_bitmap* b = Lunar<_lua_bitmap>::check( L , 3 );
	if( !b )
		return 0;
	_bitmapPort::copyHorizontalStretch( 
		luaL_checkint( L , 1 ) ,
		luaL_checkint( L , 2 ) ,
		luaL_checkint( L , 3 ) ,
		*b->bm
	);
	return 0;
}

//! Lua-_gadget
const char _lua_bitmapPort::className[] = "_bitmapPort";
Lunar<_lua_bitmapPort>::FunctionType _lua_bitmapPort::methods[] = {
	LUA_CLASS_FUNC(_lua_bitmapPort, addClippingRects),
	LUA_CLASS_FUNC(_lua_bitmapPort, normalizeClippingRects),
	LUA_CLASS_FUNC(_lua_bitmapPort, deleteClippingRects),
	LUA_CLASS_FUNC(_lua_bitmapPort, getPixel),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawPixel),
	LUA_CLASS_FUNC(_lua_bitmapPort, fill),
	LUA_CLASS_FUNC(_lua_bitmapPort, replaceColor),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawLine),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawVerticalLine),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawVerticalDottedLine),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawHorizontalLine),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawHorizontalDottedLine),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawRect),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawFilledRect),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawVerticalGradient),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawHorizontalGradient),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawCircle),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawFilledCircle),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawEllipse),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawFilledEllipse),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawChar),
	LUA_CLASS_FUNC(_lua_bitmapPort, drawString),
	LUA_CLASS_FUNC(_lua_bitmapPort, copy),
	LUA_CLASS_FUNC(_lua_bitmapPort, copyTransparent),
	LUA_CLASS_FUNC(_lua_bitmapPort, copyHorizontalStretch),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_bitmapPort>::PropertyType _lua_bitmapPort::properties[] = {
	{ "width" , &_lua_bitmapPort::getWidth , nullptr },
	{ "height" , &_lua_bitmapPort::getHeight , nullptr },
	LUA_CLASS_ATTR_END
};