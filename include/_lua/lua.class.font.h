// Check if already included
#ifndef _WIN_LUAFONT_
#define _WIN_LUAFONT_

#include "_lua/lunar.h"
#include "_type/type.freetypefont.h"

/**
 * Proxy Classes
 */
class _lua_font{
	
	public:
		
		_freetypefont*	font;
		
		//! Ctor
		_lua_font( _freetypefont* f );
		
		//! Lua-Ctor
		_lua_font( lua_State* L );
		
		//! getStringWidth
		int getStringWidth( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_font>::FunctionType methods[];
		static Lunar<_lua_font>::PropertyType properties[];
};

#endif