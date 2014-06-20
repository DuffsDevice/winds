// Check if already included
#ifndef _WIN_L_MAINFRAME_
#define _WIN_L_MAINFRAME_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.window.mainframe.h"

/**
 * Proxy Classes
 */
class _lua_mainframe : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_mainframe( lua_State* L );
		
		//! C-Ctor
		_lua_mainframe( _mainFrame* w ) : _lua_gadget( w , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_mainframe>::FunctionType methods[];
		static Lunar<_lua_mainframe>::PropertyType properties[];
		using baseclasses = Lunar<_lua_mainframe>::BaseClassType<_lua_gadget>;
};

#endif