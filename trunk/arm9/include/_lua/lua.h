#ifndef _WIN_L_LUAHEADERS_
#define _WIN_L_LUAHEADERS_

#include <_type/type.h>

// Lua header files for C++
extern "C"
{
	#include <_library/_lua/lua.h>
	#include <_library/_lua/lualib.h>
	#include <_library/_lua/lauxlib.h>
	
	static unused int lua_typeerror( lua_State* L , int narg , _literal tname ){
		const char *msg = lua_pushfstring( L , "%s expected, got %s" , tname , luaL_typename( L , narg ) );
		return luaL_argerror( L , narg , msg );
	}
	static unused int lua_tagerror( lua_State* L , int narg , int tag ){
		return lua_typeerror( L , narg , lua_typename( L , tag ) );
	}
	static unused int lua_notnoneerror( lua_State* L , int narg ){
		const char *msg = lua_pushfstring( L , "Expected no parameter, got %s" , luaL_typename( L , narg ) );
		return luaL_argerror( L , narg , msg );
	}
	static unused int constexpr lua_toAbsIndex( lua_State* L , int idx ){
		return (idx > 0) ? idx : (idx <= LUA_REGISTRYINDEX) ? idx : (lua_gettop(L) + 1 + idx);
	}
	static unused int lua_convToAbsIndex( lua_State* L , int& idx ){
		return (idx = (idx > 0) ? idx : (idx <= LUA_REGISTRYINDEX) ? idx : (lua_gettop(L) + 1 + idx));
	}
	#define lua_rawgetfield( L , idx , field ) lua_pushliteral( L , field ); lua_rawget( L , idx )
}

#endif