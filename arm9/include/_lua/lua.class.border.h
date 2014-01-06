// Check if already included
#ifndef _WIN_L_BORDER_
#define _WIN_L_BORDER_

#include "_lua/lua.lunar.h"
#include "_type/type.h"

/**
 * Proxy Classes
 */
class _lua_border : public _border
{
	public:
		
		_lua_border( _border rc );
		
		//! Constructor
		_lua_border( lua_State* L );
		
		//! getLeft
		int getLeft(lua_State* L);
		
		//! getTop
		int getTop(lua_State* L);
		
		//! getRight
		int getRight(lua_State* L);
		
		//! getBottom
		int getBottom(lua_State* L);
		
		//! setLeft
		int setLeft(lua_State* L);
		
		//! setTop
		int setTop(lua_State* L);
		
		//! setRight
		int setRight(lua_State* L);
		
		//! setBottom
		int setBottom(lua_State* L);
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_border>::FunctionType methods[];
		static Lunar<_lua_border>::PropertyType properties[];
	
};

#endif