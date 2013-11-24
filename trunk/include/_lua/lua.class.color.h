// Check if already included
#ifndef _WIN_L_COLOR_
#define _WIN_L_COLOR_

#include "_lua/lua.lunar.h"
#include "_type/type.h"
#include "_type/type.color.h"

/**
 * Proxy Classes
 */
class _lua_color : public _color {
	
	public:
		
		_lua_color( _color rc );
		
		//! Constructor
		_lua_color( lua_State* L );
		
		//! getR
		int getR(lua_State* L);
		
		//! getG
		int getG(lua_State* L);
		
		//! getB
		int getB(lua_State* L);
		
		//! getH
		int getH(lua_State* L);
		
		//! getS
		int getS(lua_State* L);
		
		//! getL
		int getL(lua_State* L);
		
		//! getColor
		int getColor(lua_State* L);
		
		//! setR
		int setR(lua_State* L);
		
		//! setG
		int setG(lua_State* L);
		
		//! setB
		int setB(lua_State* L);
		
		//! setH
		int setH(lua_State* L);
		
		//! setS
		int setS(lua_State* L);
		
		//! setL
		int setL(lua_State* L);
		
		//! setColor
		int setColor(lua_State* L);
		
		//! setRGB
		int setRGB(lua_State* L);
		
		//! setHSL
		int setHSL(lua_State* L);
		
		//! adjustRGB
		int adjustRGB(lua_State* L);
		
		//! adjustHSL
		int adjustHSL(lua_State* L);
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_color>::FunctionType methods[];
		static Lunar<_lua_color>::PropertyType properties[];
	
};

#endif