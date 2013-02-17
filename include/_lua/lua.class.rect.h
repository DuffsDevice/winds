// Check if already included
#ifndef _WIN_LUARECT_
#define _WIN_LUARECT_

#include "_lua/lunar.h"
#include "_type/type.rect.h"

/**
 * Proxy Classes
 
 */
class _lua_rect : public _rect {
		
	public:
		
		_lua_rect( _rect rc );
		
		//! Constructor
		_lua_rect( lua_State* L );
		
		//! Set X
		int setX(lua_State* L);
		
		//! Set Y
		int setY(lua_State* L);
		
		//! Get X
		int getX(lua_State* L);
		
		//! Get Y
		int getY(lua_State* L);
		
		//! Set X2
		int setX2(lua_State* L);
		
		//! Set Y2
		int setY2(lua_State* L);
		
		//! Get X2
		int getX2(lua_State* L);
		
		//! Get Y2
		int getY2(lua_State* L);
		
		//! Set Width
		int setWidth(lua_State* L);
		
		//! Set Height
		int setHeight(lua_State* L);
		
		//! Get Width
		int getWidth(lua_State* L);
		
		//! Get Height
		int getHeight(lua_State* L);
		
		//! isValid
		int isValid(lua_State* L);
		
		//! contains (Coords)
		int contains(lua_State* L);
		
		//! fromCoords
		int fromCoords(lua_State* L);
		
		//! toRelative
		int toRelative(lua_State* L);
		
		//! clipToIntersect
		int clipToIntersect(lua_State* L);
		
		//! expandToInclude
		int expandToInclude(lua_State* L);
		
		//! reduce
		int reduce(lua_State* L);
		
		//! combine
		int combine(lua_State* L);
		
		//! intersectsWith
		int intersectsWith(lua_State* L);
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_rect>::FunctionType methods[];
		static Lunar<_lua_rect>::PropertyType properties[];
	
};

#endif