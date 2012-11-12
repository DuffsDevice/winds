// Check if already included
#ifndef _WIN_LUAIMAGEGADGET_
#define _WIN_LUAIMAGEGADGET_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.bitmap.h"
#include "_gadget/gadget.imagegadget.h"

/**
 * Proxy Classes
**/
class _lua_imagegadget : public _lua_gadget {
	
	public:
		
		_lua_imagegadget( lua_State* L );
		
		_lua_imagegadget( _imagegadget* c ) : _lua_gadget( c ) { }
		
		int getImage( lua_State* L );
		
		int setImage( lua_State* L );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_imagegadget>::FunctionType methods[];
		static Lunar<_lua_imagegadget>::PropertyType properties[];
	
};

#endif