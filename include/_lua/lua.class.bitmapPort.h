// Check if already included
#ifndef _WIN_LUABITMAPPORT_
#define _WIN_LUABITMAPPORT_

extern "C"{
	#include "_lua/lua.h"
	#include "_lua/lauxlib.h"
	#include "_lua/lualib.h"
}
#include "_lua/lunar.h"
#include "_lua/lua.class.font.h"
#include "_lua/lua.class.rect.h"
#include "_type/type.bitmapPort.h"

extern bool luaL_is( lua_State* L , int narg , string type );


/**
 * Proxy Classes
**/
class _lua_bitmapPort : public _bitmapPort{
	
	public:
		
		// Ctor
		_lua_bitmapPort( _bitmapPort b );
		
		// Lua-Ctor
		_lua_bitmapPort( lua_State* L );
		
		//! addClippingRects
		int addClippingRects( lua_State* L );
		
		//! resetClippingRects
		int resetClippingRects( lua_State* L );
		
		//! deleteClippingRects
		int deleteClippingRects( lua_State* L );
		
		//! opertor[pos] and operator(x,y)
		int get( lua_State* L );
		
		//! getWidth
		int getWidth(lua_State* L );
		
		//! getHeight
		int getHeight(lua_State* L );
		
		//! getPixel
		int getPixel( lua_State* L );
		
		//! drawPixel
		int drawPixel( lua_State* L );
		
		//! fill
		int fill( lua_State* L );
		
		//! drawVerticalLine
		int drawVerticalLine( lua_State* L );
		
		//! drawHorizontalLine
		int drawHorizontalLine( lua_State* L );
		
		//! drawRect
		int drawRect( lua_State* L );
		
		//! drawFilledRect
		int drawFilledRect( lua_State* L );
		
		//! drawVerticalGradient
		int drawVerticalGradient( lua_State* L );
		
		//! drawHorizontalGradient
		int drawHorizontalGradient( lua_State* L );
		
		//! drawCircle
		int drawCircle( lua_State* L );
		
		//! drawFilledCircle
		int drawFilledCircle( lua_State* L );
		
		//! drawEllipse
		int drawEllipse( lua_State* L );
		
		//! drawFilledEllipse
		int drawFilledEllipse( lua_State* L );
		
		//! drawChar
		int drawChar( lua_State* L );
			
		//! drawString
		int drawString( lua_State* L );
		
		//! copy
		int copy( lua_State* L );
		
		//! copyTransparent
		int copyTransparent( lua_State* L );
		
		//! copyHorizontalStretch
		int copyHorizontalStretch( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_bitmapPort>::RegType methods[];
};

#endif