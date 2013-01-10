// Check if already included
#ifndef _WIN_LUARADIOGROUP_
#define _WIN_LUARADIOGROUP_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_type/type.radiogroup.h"

class _lua_radiogroup : public _radiogroup {
	
	public:
		
		//! Lua-Ctor...
		_lua_radiogroup( lua_State* L );
		
		//! addRadio
		int addRadio(lua_State* L);
		
		//! removeRadio
		int removeRadio(lua_State* L);
		
		//! enableRadio
		int enableRadio(lua_State* L);
		
		//! setIntValue
		int setIntValue(lua_State* L);
		
		//! getIntValue
		int getIntValue(lua_State* L);
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_radiogroup>::FunctionType methods[];
		static Lunar<_lua_radiogroup>::PropertyType properties[];
};

#endif