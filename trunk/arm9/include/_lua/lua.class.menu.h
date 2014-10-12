// Check if already included
#ifndef _WIN_L_MENU_
#define _WIN_L_MENU_

#include <_lua/lua.lunar.h>
#include <_type/type.h>
#include <_type/type.menu.h>

/**
 * Proxy Classes
 */
class _lua_menu
{
	private:
		
		_menu*	menu;
		bool	wasAllocated;
		
	public:
		
		//! Constructor
		_lua_menu( lua_State* L );
		
		//! C++ - Ctors
		_lua_menu( _menu& menu ) :
			menu( &menu )
			, wasAllocated( false )
		{}
		_lua_menu( _menu&& menu ) :
			menu( new _menu( move(menu) ) )
			, wasAllocated( true )
		{}
		
		//! Cast to _menu&
		operator _menu&(){ return *this->menu; }
		
		//! addHandler
		int addHandler(lua_State* L);
		
		//! setList
		int setList(lua_State* L);
		
		//! getList
		int getList(lua_State* L);
		
		//! callHandler
		int callHandler(lua_State* L);
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_menu>::FunctionType methods[];
		static Lunar<_lua_menu>::PropertyType properties[];
	
};

#endif
