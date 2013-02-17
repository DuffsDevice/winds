// Check if already included
#ifndef _WIN_LUAIMAGEFILE_
#define _WIN_LUAIMAGEFILE_

#include "_lua/lunar.h"
#include "_type/type.h"
#include "_type/type.imagefile.h"

/**
 * Proxy Classes
 */
class _lua_imagefile : public _imagefile {
	
	public:
		
		//! Constructor
		_lua_imagefile( lua_State* L );
		
		//! getBitmap
		int getBitmap(lua_State* L);
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_imagefile>::FunctionType methods[];
		static Lunar<_lua_imagefile>::PropertyType properties[];
	
};

#endif
