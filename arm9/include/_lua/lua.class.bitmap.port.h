// Check if already included
#ifndef _WIN_L_BITMAPPORT_
#define _WIN_L_BITMAPPORT_

#include <_lua/lua.lunar.h>
#include <_type/type.bitmap.port.h>

/**
 * Proxy Classes
 */
class _lua_bitmapport : public _bitmapPort
{
	public:
		
		//! Lua-Ctor
		_lua_bitmapport( lua_State* L );
		
		//! C-Ctor
		_lua_bitmapport( _bitmapPort b );
		
		//! addClippingRects
		int addClippingRects( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_bitmapport>::FunctionType methods[];
		static Lunar<_lua_bitmapport>::PropertyType properties[];
};

#endif