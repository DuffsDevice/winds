// Check if already included
#ifndef _WIN_L_IMAGEFILE_
#define _WIN_L_IMAGEFILE_

#include <_lua/lua.lunar.h>
#include <_lua/lua.class.direntry.h>
#include <_type/type.h>
#include <_type/type.imagefile.h>

/**
 * Proxy Classes
 */
class _lua_imagefile : public _imageFile
{
	public:
		
		//! Constructor
		_lua_imagefile( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_imagefile>::FunctionType methods[];
		static Lunar<_lua_imagefile>::PropertyType properties[];
		using baseclasses = Lunar<_lua_imagefile>::BaseClassType<_lua_direntry>;
};

#endif
