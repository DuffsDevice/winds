// Check if already included
#ifndef _WIN_L_FILE_
#define _WIN_L_FILE_

#include <_lua/lua.lunar.h>
#include <_type/type.direntry.h>

/**
 * Proxy Classes
 */
class _lua_direntry : public _direntry {
	
	public:
		
		//! Ctor
		_lua_direntry( const _direntry &f );
		
		//! Lua-Ctor
		_lua_direntry( lua_State* L );
		
		//! openWrite
		int openWrite( lua_State* L );
		
		//! getFullName
		int getFullName( lua_State* L );
		
		//! For Directoryies
		int readChild( lua_State* L );
		int readChildFolderOnly( lua_State* L );
		
		//! execute
		int execute( lua_State* L );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_direntry>::FunctionType	methods[];
		static Lunar<_lua_direntry>::PropertyType	properties[];
};

#endif