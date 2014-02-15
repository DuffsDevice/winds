#ifndef _WIN_L_MIME_
#define _WIN_L_MIME_

#include "_lua/lua.lunar.h"
#include "_type/type.mime.h"

/**
 * Proxy Classes
 */
class _lua_mimetype : public _mimeType
{
	public:
	
		//! Constructor
		_lua_mimetype( lua_State* L );
		
		//! C-Ctor
		_lua_mimetype( const _mimeType& );
		
		
		//! equals
		int equals( lua_State* L );
		
		//! fromExtension
		static int fromExtension( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_mimetype>::FunctionType	methods[];
		static Lunar<_lua_mimetype>::PropertyType	properties[];
		static Lunar<_lua_mimetype>::StaticType		staticmethods[];
};

#endif