// Check if already included
#ifndef _WIN_L_AREA_
#define _WIN_L_AREA_

#include <_lua/lua.lunar.h>
#include <_type/type.rect.h>

class _lua_area : public _area {
	
	public:
		
		_lua_area( _area a );
		
		//! Constructor
		_lua_area( lua_State* L );
		
		//! Push-back
		int add( lua_State* L );
		
		//! reduce
		int reduce( lua_State* L );
		
		//! clear
		int clearRects(lua_State* L);
		
		//! toRelative
		int toRelative(lua_State* L);
		
		//! clipToIntersect
		int clipToIntersect(lua_State* L);
		
		//! dump
		int dump(lua_State* L);
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_area>::FunctionType methods[];
		static Lunar<_lua_area>::PropertyType properties[];
};

#endif