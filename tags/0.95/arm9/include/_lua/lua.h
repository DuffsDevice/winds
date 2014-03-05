#ifndef _WIN_L_LUAHEADERS_
#define _WIN_L_LUAHEADERS_

#include "_type/type.h"

// Lua header files for C++
extern "C"
{
	#include "_library/_lua/lua.h"
	#include "_library/_lua/lualib.h"
	#include "_library/_lua/lauxlib.h"
	
	static unused int lua_typeerror( lua_State* L , int narg , const char* tname ){
		const char *msg = lua_pushfstring( L , "%s expected, got %s" , tname , luaL_typename( L , narg ) );
		return luaL_argerror( L , narg , msg );
	}
	static unused void lua_tagerror( lua_State* L , int narg , int tag ){
		lua_typeerror( L , narg , lua_typename( L , tag ) );
	}
}

#endif