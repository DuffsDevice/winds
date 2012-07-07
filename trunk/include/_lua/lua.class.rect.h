// Check if already included
#ifndef _WIN_LUARECTAREA_
#define _WIN_LUARECTAREA_

extern "C"{
	#include "_lua/lua.h"
	#include "_lua/lauxlib.h"
	#include "_lua/lualib.h"
}
#include "_lua/lunar.h"
#include "_type/type.rect.h"

/**
 * Proxy Classes
**/
class _lua_rect : public _rect {	
	
	public:
		
		class _lua_area : public _area	{
			public:
				
				_lua_area( _area a );
				
				//! Constructor
				_lua_area( lua_State* L );
				
				//! Push-back
				int insert( lua_State* L );
				
				//! clear
				int clear(lua_State* L);
				
				//! toRelative
				int toRelative(lua_State* L);
				
				//! Lunar !//
				static const char className[];
				static Lunar<_lua_area>::RegType methods[];
		};
		
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
		
		//! Contains (Coords)
		int contains(lua_State* L);
		
		//! fromCoords
		int fromCoords(lua_State* L);
		
		//! toRelative
		int toRelative(lua_State* L);
		
		//! AND
		int AND(lua_State* L);
		
		//! +
		int ADD(lua_State* L);
		
		//! -
		int SUB(lua_State* L);
		
		//! XOR
		int XOR(lua_State* L);
		
		//! OR
		int OR(lua_State* L);
		
		//! intersectsWith
		int intersectsWith(lua_State* L);
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_rect>::RegType methods[];
	
};

typedef _lua_rect::_lua_area _lua_area;

#endif