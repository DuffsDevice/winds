// Check if already included
#ifndef _WIN_L_BITMAPPORT_
#define _WIN_L_BITMAPPORT_

#include "_lua/lua.lunar.h"
#include "_type/type.bitmap.port.h"

/**
 * Proxy Classes
 */
class _lua_bitmapPort : public _bitmapPort
{
	public:
		
		//! Lua-Ctor
		_lua_bitmapPort( lua_State* L );
		
		//! C-Ctor
		_lua_bitmapPort( _bitmapPort b );
		
		//! addClippingRects
		int addClippingRects( lua_State* L );
		
		int dummy( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_bitmapPort>::FunctionType methods[];
		static Lunar<_lua_bitmapPort>::PropertyType properties[];
};

#endif