#include "_lua/lua.class.bitmapPort.h"
#include "_lua/lua.class.bitmap.h"

/*##################################
##         Lua-Bitmap-Port        ##
##################################*/

// Ctor
_lua_bitmapPort::_lua_bitmapPort( _bitmapPort b ) :
	_bitmapPort( b )
{}

// Lua-Ctor
_lua_bitmapPort::_lua_bitmapPort( lua_State* L ) :
	_bitmapPort( Lunar<_lua_bitmap>::check( L , 3 )->bm )
{ }

//! opertor[pos] and operator(x,y)
int _lua_bitmapPort::get( lua_State* L ){ 
	if( lua_gettop(L) == 1 )
		lua_pushnumber( L , _bitmapPort::operator[]( luaL_checkint( L , 1 ) ) );
	else
		lua_pushnumber( L , _bitmapPort::operator()( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) ) );
	return 1;
}

int _lua_bitmapPort::addClippingRects( lua_State* L ){
	if( luaL_is( L , 1 , "_rect" ) )
		_bitmapPort::addClippingRects( *Lunar<_lua_rect>::check( L , 1 ) );
	else
		_bitmapPort::addClippingRects( *Lunar<_lua_area>::check( L , 1 ) );
	return 0;
}

int _lua_bitmapPort::resetClippingRects( lua_State* L ){
	_bitmapPort::resetClippingRects();
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
int _lua_bitmapPort::drawPixel( lua_State* L ){ _bitmapPort::drawPixel( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) ); return 0; }

//! fill
int _lua_bitmapPort::fill( lua_State* L ){ _bitmapPort::fill( luaL_checkint( L , 1 ) ); return 0; }

//! drawVerticalLine
int _lua_bitmapPort::drawVerticalLine( lua_State* L ){ _bitmapPort::drawVerticalLine( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) ); return 0; }

//! drawHorizontalLine
int _lua_bitmapPort::drawHorizontalLine( lua_State* L ){ _bitmapPort::drawHorizontalLine( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) ); return 0; }

//! drawRect
int _lua_bitmapPort::drawRect( lua_State* L ){ _bitmapPort::drawRect( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkint( L , 5 )); return 0; }

//! drawFilledRect
int _lua_bitmapPort::drawFilledRect( lua_State* L ){ _bitmapPort::drawFilledRect( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkint( L , 5 )); return 0; }

//! drawVerticalGradient
int _lua_bitmapPort::drawVerticalGradient( lua_State* L ){ 
	_bitmapPort::drawVerticalGradient( 
		luaL_checkint( L , 1 )
		, luaL_checkint( L , 2 )
		, luaL_checkint( L , 3 )
		, luaL_checkint( L , 4 )
		, luaL_checkint( L , 5 )
		, luaL_checkint( L , 6 )
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
		, luaL_checkint( L , 5 )
		, luaL_checkint( L , 6 )
	); 
	return 0;
}

//! drawCircle
int _lua_bitmapPort::drawCircle( lua_State* L ){ _bitmapPort::drawCircle( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) ); return 0; }

//! drawFilledCircle
int _lua_bitmapPort::drawFilledCircle( lua_State* L ){ _bitmapPort::drawFilledCircle( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) ); return 0; }

//! drawEllipse
int _lua_bitmapPort::drawEllipse( lua_State* L ){ _bitmapPort::drawEllipse( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkint( L , 5 ) ); return 0; }

//! drawFilledEllipse
int _lua_bitmapPort::drawFilledEllipse( lua_State* L ){ _bitmapPort::drawFilledEllipse( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkint( L , 5 ) ); return 0; }

//! drawChar
int _lua_bitmapPort::drawChar( lua_State* L ){ 
	_font* f = Lunar<_lua_font>::check( L , 3 )->font;
	if( !f )
		return 0;
	lua_pushnumber( L , _bitmapPort::drawChar( 
		luaL_checkint( L , 1 ) ,
		luaL_checkint( L , 2 ) ,
		f ,
		luaL_checkstring( L , 4 )[0] ,
		luaL_checkint( L , 5 ) 
	) ); return 1;
}
	
//! drawString
int _lua_bitmapPort::drawString( lua_State* L )
{ 
	_font* f = Lunar<_lua_font>::check( L , 3 )->font;
	if( !f )
		return 0;
	_bitmapPort::drawString( 
		luaL_checkint( L , 1 ) ,
		luaL_checkint( L , 2 ) ,
		f ,
		luaL_checkstring( L , 4 ) ,
		luaL_checkint( L , 5 ) 
	); return 0; 
}

//! copy
int _lua_bitmapPort::copy( lua_State* L )
{ 
	if( luaL_is( L , 3 , "_bitmap" ) ){
		_lua_bitmap* b = Lunar<_lua_bitmap>::check( L , 3 );
		if( !b )
			return 0;
		_bitmapPort::copy( luaL_checkint( L , 1 ) ,	luaL_checkint( L , 2 ) ,	b->bm );
	}
	else{
		_lua_bitmapPort* b = Lunar<_lua_bitmapPort>::check( L , 3 );
		if( !b )
			return 0;
		_bitmapPort::copy( luaL_checkint( L , 1 ) ,	luaL_checkint( L , 2 ) ,	*b );
	} 
	return 0;
}

//! copyTransparent
int _lua_bitmapPort::copyTransparent( lua_State* L )
{ 
	if( luaL_is( L , 3 , "_bitmap" ) ){
		_lua_bitmap* b = Lunar<_lua_bitmap>::check( L , 3 );
		if( !b )
			return 0;
		_bitmapPort::copyTransparent( 
			luaL_checkint( L , 1 ) ,
			luaL_checkint( L , 2 ) ,
			b->bm ,
			luaL_optint( L , 4 , 0 )
		);
	}
	else{
		_lua_bitmapPort* b = Lunar<_lua_bitmapPort>::check( L , 3 );
		if( !b )
			return 0;
		_bitmapPort::copyTransparent( 
			luaL_checkint( L , 1 ) ,
			luaL_checkint( L , 2 ) ,
			*b ,
			luaL_optint( L , 4 , 0 )
		);
	}
	return 0;
}

//! copyHorizontalStretch
int _lua_bitmapPort::copyHorizontalStretch( lua_State* L )
{ 
	if( luaL_is( L , 3 , "_bitmap" ) ){
		_lua_bitmap* b = Lunar<_lua_bitmap>::check( L , 3 );
		if( !b )
			return 0;
		_bitmapPort::copyHorizontalStretch( 
			luaL_checkint( L , 1 ) ,
			luaL_checkint( L , 2 ) ,
			luaL_checkint( L , 3 ) ,
			b->bm
		);
	}
	else{
		_lua_bitmapPort* b = Lunar<_lua_bitmapPort>::check( L , 3 );
		if( !b )
			return 0;
		_bitmapPort::copyHorizontalStretch( 
			luaL_checkint( L , 1 ) ,
			luaL_checkint( L , 2 ) ,
			luaL_checkint( L , 3 ) ,
			*b
		);
	}
	return 0;
}

//! Lua-_gadget
const char _lua_bitmapPort::className[] = "_bitmapPort";
Lunar<_lua_bitmapPort>::RegType _lua_bitmapPort::methods[] = {
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, addClippingRects),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, resetClippingRects),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, deleteClippingRects),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, get),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, getWidth),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, getHeight),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, getPixel),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, drawPixel),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, fill),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, drawVerticalLine),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, drawHorizontalLine),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, drawRect),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, drawFilledRect),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, drawVerticalGradient),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, drawHorizontalGradient),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, drawCircle),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, drawFilledCircle),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, drawEllipse),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, drawFilledEllipse),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, drawChar),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, drawString),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, copy),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, copyTransparent),
	LUNAR_DECLARE_METHOD(_lua_bitmapPort, copyHorizontalStretch),
	{0,0}
};