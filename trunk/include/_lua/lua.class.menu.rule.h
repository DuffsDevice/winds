// Check if already included
#ifndef _WIN_L_MENURULE_
#define _WIN_L_MENURULE_

#include "_lua/lua.lunar.h"
#include "_type/type.h"
#include "_type/type.menu.rule.h"

/**
 * Proxy Classes
 */
class _lua_menurule : public _menuHandlerRule {
	
	public:
		
		//! Constructor
		_lua_menurule( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_menurule>::FunctionType methods[];
		static Lunar<_lua_menurule>::PropertyType properties[];
	
};

#endif
