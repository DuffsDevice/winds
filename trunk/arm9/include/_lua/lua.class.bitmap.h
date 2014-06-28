// Check if already included
#ifndef _WIN_L_BITMAP_
#define _WIN_L_BITMAP_

#include "_lua/lua.lunar.h"
#include "_type/type.bitmap.h"

/**
 * Proxy Classes
 */
class _lua_bitmap
{
	friend class _lua_bitmapport;
	
	private:
		
		_bitmap* 	bm;
		bool 		wasAllocated;
	
	public:
		
		operator _bitmap&(){
			return *this->bm;
		}
		
		//! C-Ctor
		_lua_bitmap( _bitmap&& b );
		_lua_bitmap( _bitmap& b );
		
		//! Lua-Ctor
		_lua_bitmap( lua_State* L );
		
		//! Lua-Dtor
		~_lua_bitmap();
		
		//! opertor[pos] and operator(x,y)
		int get( lua_State* L );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_bitmap>::FunctionType methods[];
		static Lunar<_lua_bitmap>::PropertyType properties[];
};

#endif