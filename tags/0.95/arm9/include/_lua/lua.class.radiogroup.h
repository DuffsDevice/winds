// Check if already included
#ifndef _WIN_L_RADIOGROUP_
#define _WIN_L_RADIOGROUP_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_type/type.singlevaluegroup.h"
#include "_gadget/gadget.radio.h"

class _lua_radiogroup : public _singleValueGroup<_radio> {
	
	public:
		
		//! Lua-Ctor...
		_lua_radiogroup( lua_State* L );
		
		//! addRadio
		int addRadio(lua_State* L);
		
		//! removeRadio
		int removeRadio(lua_State* L);
		
		//! enableRadio
		int enableRadio(lua_State* L);
		
		//! setValue
		int setValue( lua_State* L );
		
		//! getValue
		int getValue( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_radiogroup>::FunctionType methods[];
		static Lunar<_lua_radiogroup>::PropertyType properties[];
};

#endif