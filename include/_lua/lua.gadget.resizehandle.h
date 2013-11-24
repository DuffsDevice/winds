// Check if already included
#ifndef _WIN_L_RESIZEHANDLE_
#define _WIN_L_RESIZEHANDLE_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.resizehandle.h"

/**
 * Proxy Class
 */
class _lua_resizehandle : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_resizehandle( lua_State* L );
		
		//! C-Ctor
		_lua_resizehandle( _resizeHandle* c ) : _lua_gadget( c , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_resizehandle>::FunctionType methods[];
		static Lunar<_lua_resizehandle>::PropertyType properties[];
	
};

#endif