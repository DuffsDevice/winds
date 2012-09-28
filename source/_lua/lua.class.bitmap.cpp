#include "_lua/lua.class.bitmap.h"


/*##################################
##           Lua-Bitmap           ##
##################################*/

// Ctor
_lua_bitmap::_lua_bitmap( _bitmap* b ){ this->bm = b; }

// Lua-Ctor
_lua_bitmap::_lua_bitmap( lua_State* L ){
	if( lua_gettop(L) == 3 )
		this->bm = new _bitmap( (_pixelArray)luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) );
	else
		this->bm = new _bitmap( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) );
}

//! Dtor
_lua_bitmap::~_lua_bitmap(){ if( this->bm != NULL ){ delete this->bm; this->bm = NULL; } }

//! opertor[pos] and operator(x,y)
int _lua_bitmap::get( lua_State* L ){ 
	if( lua_gettop(L) == 1 )
		lua_pushnumber( L , this->bm->operator[]( luaL_checkint( L , 1 ) ) );
	else
		lua_pushnumber( L , this->bm->operator()( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) ) );
	return 1;
}

//! getWidth
int _lua_bitmap::getWidth(lua_State* L ){ lua_pushnumber( L , this->bm->getWidth() ); return 1; }

//! getHeight
int _lua_bitmap::getHeight(lua_State* L ){ lua_pushnumber( L , this->bm->getHeight() ); return 1; }

//! setWidth
int _lua_bitmap::setWidth( lua_State* L ){ this->bm->setWidth( luaL_checkint( L , 1 ) ); return 0; }

//! setHeight
int _lua_bitmap::setHeight( lua_State* L ){ this->bm->setHeight( luaL_checkint( L , 1 ) ); return 0; }

//! resize
int _lua_bitmap::resize( lua_State* L ){ this->bm->resize( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) ); return 0; }

//! getPixel
int _lua_bitmap::getPixel( lua_State* L ){ lua_pushnumber( L , this->bm->getPixel( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) ) ); return 1; }

//! drawPixel
int _lua_bitmap::drawPixel( lua_State* L ){ this->bm->drawPixel( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) ); return 0; }

//! reset
int _lua_bitmap::reset( lua_State* L ){ this->bm->reset( luaL_checkint( L , 1 ) ); return 0; }

//! fill
int _lua_bitmap::fill( lua_State* L ){ this->bm->fill( luaL_checkint( L , 1 ) ); return 0; }

//! drawVerticalLine
int _lua_bitmap::drawVerticalLine( lua_State* L ){ this->bm->drawVerticalLine( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) ); return 0; }

//! drawHorizontalLine
int _lua_bitmap::drawHorizontalLine( lua_State* L ){ this->bm->drawHorizontalLine( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) ); return 0; }

//! drawRect
int _lua_bitmap::drawRect( lua_State* L ){ this->bm->drawRect( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkint( L , 5 )); return 0; }

//! drawFilledRect
int _lua_bitmap::drawFilledRect( lua_State* L ){ this->bm->drawFilledRect( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkint( L , 5 )); return 0; }

//! drawVerticalGradient
int _lua_bitmap::drawVerticalGradient( lua_State* L ){ 
	this->bm->drawVerticalGradient( 
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
int _lua_bitmap::drawHorizontalGradient( lua_State* L ){ 
	this->bm->drawHorizontalGradient( 
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
int _lua_bitmap::drawCircle( lua_State* L ){ this->bm->drawCircle( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) ); return 0; }

//! drawFilledCircle
int _lua_bitmap::drawFilledCircle( lua_State* L ){ this->bm->drawFilledCircle( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) ); return 0; }

//! drawEllipse
int _lua_bitmap::drawEllipse( lua_State* L ){ this->bm->drawEllipse( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkint( L , 5 ) ); return 0; }

//! drawFilledEllipse
int _lua_bitmap::drawFilledEllipse( lua_State* L ){ this->bm->drawFilledEllipse( luaL_checkint( L , 1 ) , luaL_checkint( L , 2 ) , luaL_checkint( L , 3 ) , luaL_checkint( L , 4 ) , luaL_checkint( L , 5 ) ); return 0; }

//! drawChar
int _lua_bitmap::drawChar( lua_State* L ){ 
	/*_font* f = Lunar<_lua_font>::check( L , 3 )->font;
	if( !f )
		return 0;
	lua_pushnumber( L , this->bm->drawChar( 
		luaL_checkint( L , 1 ) ,
		luaL_checkint( L , 2 ) ,
		f ,
		luaL_checkstring( L , 4 )[0] ,
		luaL_checkint( L , 5 ) 
	) ); return 1;*/
	//! TODO: implement
}
	
//! drawString
int _lua_bitmap::drawString( lua_State* L ){ 
	/*_font* f = Lunar<_lua_font>::check( L , 3 )->font;
	if( !f )
		return 0;
	this->bm->drawString( 
		luaL_checkint( L , 1 ) ,
		luaL_checkint( L , 2 ) ,
		f ,
		luaL_checkstring( L , 4 ) ,
		luaL_checkint( L , 5 ) 
	); return 0; */
	//! TODO: implement
}

//! copy
int _lua_bitmap::copy( lua_State* L ){ 
	_lua_bitmap* b = Lunar<_lua_bitmap>::check( L , 3 );
	if( !b )
		return 0;
	this->bm->copy( luaL_checkint( L , 1 ) ,	luaL_checkint( L , 2 ) ,	b->bm ); return 0; 
}

//! copyTransparent
int _lua_bitmap::copyTransparent( lua_State* L ){ 
	_lua_bitmap* b = Lunar<_lua_bitmap>::check( L , 3 );
	if( !b )
		return 0;
	this->bm->copyTransparent( 
		luaL_checkint( L , 1 ) ,
		luaL_checkint( L , 2 ) ,
		b->bm
	); return 0; 
}

//! copyHorizontalStretch
int _lua_bitmap::copyHorizontalStretch( lua_State* L ){ 
	_lua_bitmap* b = Lunar<_lua_bitmap>::check( L , 3 );
	if( !b )
		return 0;
	this->bm->copyHorizontalStretch( 
		luaL_checkint( L , 1 ) ,
		luaL_checkint( L , 2 ) ,
		luaL_checkint( L , 3 ) ,
		b->bm
	); return 0; 
}

//! setClippingRect
int _lua_bitmap::setClippingRect( lua_State* L ){  _lua_rect* rc = Lunar<_lua_rect>::check( L , 1 ); if( rc ) this->bm->setClippingRect( *rc ); return 0; }

//! getClippingRect
int _lua_bitmap::getClippingRect( lua_State* L ){ Lunar<_lua_rect>::push( L , new _lua_rect( this->bm->getClippingRect() ) , true ); return 1; }

//! resetClippingRect
int _lua_bitmap::resetClippingRect( lua_State* L ){ this->bm->resetClippingRect(); return 0; }


//! Lua-_gadget
const char _lua_bitmap::className[] = "_bitmap";
Lunar<_lua_bitmap>::RegType _lua_bitmap::methods[] = {
	LUNAR_DECLARE_METHOD(_lua_bitmap, get),
	LUNAR_DECLARE_METHOD(_lua_bitmap, getWidth),
	LUNAR_DECLARE_METHOD(_lua_bitmap, setWidth),
	LUNAR_DECLARE_METHOD(_lua_bitmap, getHeight),
	LUNAR_DECLARE_METHOD(_lua_bitmap, setHeight),
	LUNAR_DECLARE_METHOD(_lua_bitmap, resize),
	LUNAR_DECLARE_METHOD(_lua_bitmap, getPixel),
	LUNAR_DECLARE_METHOD(_lua_bitmap, drawPixel),
	LUNAR_DECLARE_METHOD(_lua_bitmap, reset),
	LUNAR_DECLARE_METHOD(_lua_bitmap, fill),
	LUNAR_DECLARE_METHOD(_lua_bitmap, drawVerticalLine),
	LUNAR_DECLARE_METHOD(_lua_bitmap, drawHorizontalLine),
	LUNAR_DECLARE_METHOD(_lua_bitmap, drawRect),
	LUNAR_DECLARE_METHOD(_lua_bitmap, drawFilledRect),
	LUNAR_DECLARE_METHOD(_lua_bitmap, drawVerticalGradient),
	LUNAR_DECLARE_METHOD(_lua_bitmap, drawHorizontalGradient),
	LUNAR_DECLARE_METHOD(_lua_bitmap, drawCircle),
	LUNAR_DECLARE_METHOD(_lua_bitmap, drawFilledCircle),
	LUNAR_DECLARE_METHOD(_lua_bitmap, drawEllipse),
	LUNAR_DECLARE_METHOD(_lua_bitmap, drawFilledEllipse),
	LUNAR_DECLARE_METHOD(_lua_bitmap, drawChar),
	LUNAR_DECLARE_METHOD(_lua_bitmap, drawString),
	LUNAR_DECLARE_METHOD(_lua_bitmap, copy),
	LUNAR_DECLARE_METHOD(_lua_bitmap, copyTransparent),
	LUNAR_DECLARE_METHOD(_lua_bitmap, copyHorizontalStretch),
	LUNAR_DECLARE_METHOD(_lua_bitmap, getClippingRect),
	LUNAR_DECLARE_METHOD(_lua_bitmap, setClippingRect),
	LUNAR_DECLARE_METHOD(_lua_bitmap, resetClippingRect),
	{0,0}
};