// Check if already included
#ifndef _WIN_L_FONT_
#define _WIN_L_FONT_

#include <_lua/lua.lunar.h>
#include <_type/type.font.h>
#include <_type/type.font.handle.h>

/**
 * Proxy Classes
 */
class _lua_font
{
	private:
		
		_fontHandle	font;
	
	public:
		
		//! C-Ctor
		_lua_font( _fontHandle f );
		
		//! Lua-Ctor
		_lua_font( lua_State* L );
		
		//! Operator that allows casts to _font & _fontHandle
		operator _fontHandle(){
			return this->font;
		}
		operator _font&(){
			return const_cast<_font&>(*this->font);
		}
		
		//! getCharacterWidth
		int getCharacterWidth( lua_State* L );
		
		//! isCharSupported
		int isCharSupported( lua_State* L );
		
		//! getAscent
		int getAscent( lua_State* L );
		
		//! getLetterSpace
		int getLetterSpace( lua_State* L );
		
		//! getHeight
		int getHeight( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_font>::FunctionType methods[];
		static Lunar<_lua_font>::PropertyType properties[];
};

#endif