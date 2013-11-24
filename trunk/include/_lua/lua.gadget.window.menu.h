// Check if already included
#ifndef _WIN_L_WINDOWMENU_
#define _WIN_L_WINDOWMENU_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.window.menu.h"

/**
 * Proxy Class
 */
class _lua_windowmenu : public _lua_gadget
{
	public:
		
		_lua_windowmenu( lua_State* L );
		
		_lua_windowmenu( _windowMenu* m ) : _lua_gadget( m , false )
		{ }
		
		static int getStandardMenu( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_windowmenu>::FunctionType methods[];
		static Lunar<_lua_windowmenu>::PropertyType properties[];
		static Lunar<_lua_windowmenu>::StaticType	staticmethods[];
	
};

#endif