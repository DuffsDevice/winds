// Check if already included
#ifndef _WIN_L_MENU_
#define _WIN_L_MENU_

#include "_lua/lua.lunar.h"
#include "_type/type.h"
#include "_type/type.menu.h"

/**
 * Proxy Classes
 */
class _lua_menu : public _menu {
	
	public:
		
		//! Constructor
		_lua_menu( lua_State* L );
		
		//! C++ - Ctors
		_lua_menu( const _menu& menu ) :
			_menu( menu )
		{}
		_lua_menu( _menu&& menu ) :
			_menu( move(menu) )
		{}
		
		//! addMenuHandler
		int addMenuHandler(lua_State* L);
		
		//! setList
		int setList(lua_State* L);
		
		//! getList
		int getList(lua_State* L);
		
		//! clear
		int clear(lua_State* L);
		
		//! callHandler
		int callHandler(lua_State* L);
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_menu>::FunctionType methods[];
		static Lunar<_lua_menu>::PropertyType properties[];
	
};

#endif
