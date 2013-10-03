// Check if already included
#ifndef _WIN_L_BITMAP_
#define _WIN_L_BITMAP_

#include "_lua/lunar.h"
#include "_type/type.bitmap.h"

/**
 * Proxy Classes
 */
class _lua_bitmap{
	
	public:
		
		bool 		wasAllocated;
		_bitmap* 	bm;
		
		// Ctor
		_lua_bitmap( _bitmap* b );
		_lua_bitmap( _constbitmap* b );
		_lua_bitmap( _bitmap&& b );
		
		// Lua-Ctor
		_lua_bitmap( lua_State* L );
		
		//! Lua-Dtor
		~_lua_bitmap();
		
		//! opertor[pos] and operator(x,y)
		int get( lua_State* L );
		
		//! getWidth
		int getWidth(lua_State* L );
		
		//! getHeight
		int getHeight(lua_State* L );
		
		//! setWidth
		int setWidth( lua_State* L );
		
		//! setHeight
		int setHeight( lua_State* L );
		
		//! resize
		int resize( lua_State* L );
		
		//! getPixel
		int getPixel( lua_State* L );
		
		//! drawPixel
		int drawPixel( lua_State* L );
		
		//! reset
		int reset( lua_State* L );
		
		//! fill
		int fill( lua_State* L );
		
		//! replaceColor
		int replaceColor( lua_State* L );
		
		//! drawVerticalLine
		int drawVerticalLine( lua_State* L );
		
		//! drawHorizontalLine
		int drawHorizontalLine( lua_State* L );
		
		//! drawVerticalDottedLine
		int drawVerticalDottedLine( lua_State* L );
		
		//! drawHorizontalDottedLine
		int drawHorizontalDottedLine( lua_State* L );
		
		//! drawLine
		int drawLine( lua_State* L );
		
		//! drawRect
		int drawRect( lua_State* L );
		
		//! drawDottedRect
		int drawDottedRect( lua_State* L );
		
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
		
		//! setClippingRect
		int setClippingRect( lua_State* L );
		
		//! getClippingRect
		int getClippingRect( lua_State* L );
		
		//! resetClippingRect
		int resetClippingRect( lua_State* L );
		
		//! isValid
		int isValid( lua_State* L );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_bitmap>::FunctionType methods[];
		static Lunar<_lua_bitmap>::PropertyType properties[];
};

#endif