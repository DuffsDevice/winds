// Check if already included
#ifndef _WIN_L_FONT_
#define _WIN_L_FONT_

#include "_lua/lunar.h"
#include "_type/type.font.h"

/**
 * Proxy Classes
 */
class _lua_font{
	
	public:
		
		const _font*	font;
		
		//! Ctor
		_lua_font( const _font* f );
		
		//! Lua-Ctor
		_lua_font( lua_State* L );
		
		//! getStringWidth
		int getStringWidth( lua_State* L );
		
		//! getCharacterWidth
		int getCharacterWidth( lua_State* L );
		
		//! getNumCharsUntilWidth
		int getNumCharsUntilWidth( lua_State* L );
		
		//! isMonospace
		int isMonospace( lua_State* L );
		
		//! isCharSupported
		int isCharSupported( lua_State* L );
		
		//! getHeight
		int getHeight( lua_State* L );
		
		//! getAscent
		int getAscent( lua_State* L );
		
		//! getName
		int getName( lua_State* L );
		
		//! isValid
		int isValid( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_font>::FunctionType methods[];
		static Lunar<_lua_font>::PropertyType properties[];
};

#endif