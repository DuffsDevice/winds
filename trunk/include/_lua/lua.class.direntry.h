// Check if already included
#ifndef _WIN_LUAFILE_
#define _WIN_LUAFILE_

extern "C"{
	#include "_lua/lua.h"
	#include "_lua/lauxlib.h"
	#include "_lua/lualib.h"
}
#include "_lua/lunar.h"
#include "_type/type.direntry.h"

/**
 * Proxy Classes
**/
class _lua_direntry : public _direntry {
	
	public:
		
		//! Ctor
		_lua_direntry( _direntry &f );
		
		//! Lua-Ctor
		_lua_direntry( lua_State* L );
		
		//! open
		int open( lua_State* L );
		
		//! openwrite
		int openwrite( lua_State* L );
		
		//! openread
		int openread( lua_State* L );
		
		//! create
		int create( lua_State* L );
		
		//! exists
		int exists( lua_State* L );
		
		//! close
		int close( lua_State* L );
		
		//! For Directoryies
		int readChild( lua_State* L );
		int rewindChildren( lua_State* L );
		
		//! readString
		int readString( lua_State* L );
		
		//! writeString
		int writeString( lua_State* L );
		
		//! setAttrs
		int setAttrs( lua_State* L );
		
		//! getAttrs
		int getAttrs( lua_State* L );
		
		//! getFileName
		int getFileName( lua_State* L );
		
		//! getName
		int getName( lua_State* L );
		
		//! getExtension
		int getExtension( lua_State* L );
		
		//! getMimeTypeString
		int getMimeTypeString( lua_State* L );
		
		//! getSize
		int getSize( lua_State* L );
		
		//! execute
		int execute( lua_State* L );
		
		//! getFileImage
		int getFileImage( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_direntry>::FunctionType methods[];
		static Lunar<_lua_direntry>::PropertyType properties[];
};

#endif