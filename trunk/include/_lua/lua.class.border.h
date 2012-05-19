// Check if already included
#ifndef _WIN_LUABORDER_
#define _WIN_LUABORDER_

extern "C"{
	#include "_lua/lua.h"
	#include "_lua/lauxlib.h"
	#include "_lua/lualib.h"
}
#include "_lua/lunar.h"
#include "_type/type.h"

/**
 * Proxy Classes
**/
class _lua_border : public _border {	
		
	public:
		
		_lua_border( _border rc );
		
		//! Constructor
		_lua_border( lua_State* L);
		
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
		static Lunar<_lua_border>::RegType methods[];
	
};

#endif