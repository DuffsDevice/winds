// Check if already included
#ifndef _WIN_LUAAREA_
#define _WIN_LUAAREA_

#include "_lua/lunar.h"
#include "_type/type.rect.h"

class _lua_area : public _area {
	
	public:
		
		_lua_area( _area a );
		
		//! Constructor
		_lua_area( lua_State* L );
		
		//! Push-back
		int add( lua_State* L );
		
		//! reduce
		int reduce( lua_State* L );
		
		//! clearRects
		int clearRects(lua_State* L);
		
		//! relativate
		int relativate(lua_State* L);
		
		//! clipToIntersect
		int clipToIntersect(lua_State* L);
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_area>::FunctionType methods[];
		static Lunar<_lua_area>::PropertyType properties[];
};

#endif