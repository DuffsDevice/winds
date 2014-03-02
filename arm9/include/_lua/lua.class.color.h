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
		_lua_color(lua_State* L);
		
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
		
		
		//! fromHex
		static int fromHex(lua_State* L);
		
		//! fromRGB
		static int fromRGB(lua_State* L);
		
		//! fromRGB255
		static int fromRGB8(lua_State* L);
		
		//! fromBW
		static int fromBW(lua_State* L);
		
		//! fromBW255
		static int fromBW8(lua_State* L);
		
		//! fromHSL
		static int fromHSL(lua_State* L);
		
		//! mix
		static int mix(lua_State* L);
		
		//! distance
		static int distance(lua_State* L);
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_color>::FunctionType	methods[];
		static Lunar<_lua_color>::PropertyType	properties[];
		static Lunar<_lua_color>::StaticType	staticmethods[];
		
};

#endif