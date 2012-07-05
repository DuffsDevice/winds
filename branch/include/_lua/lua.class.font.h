// Check if already included
#ifndef _WIN_LUAFONT_
#define _WIN_LUAFONT_

extern "C"{
	#include "_lua/lua.h"
	#include "_lua/lauxlib.h"
	#include "_lua/lualib.h"
}
#include "_lua/lunar.h"
#include "_type/type.freetypefont.h"

/**
 * Proxy Classes
**/
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
		static Lunar<_lua_font>::RegType methods[];
};

#endif