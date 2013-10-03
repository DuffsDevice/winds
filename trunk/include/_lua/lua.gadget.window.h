// Check if already included
#ifndef _WIN_L_WINDOW_
#define _WIN_L_WINDOW_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.window.h"

/**
 * Proxy Classes
 */
class _lua_window : public _lua_gadget , public _lua_interface_input<_window> {	
	
	public:
		
		_lua_window( lua_State* L );
		
		_lua_window( _window* w ) : _lua_gadget( w , false )
		{ }
		
		//! setIcon
		int setIcon( lua_State* L );
		
		//! hasIcon
		int hasIcon( lua_State* L );
		
		//! getIcon
		int getIcon( lua_State* L );
		
		//! isMinimizeable
		int isMinimizeable( lua_State* L );
		
		//! setMinimizeable
		int setMinimizeable( lua_State* L );
		
		//! isMinimized
		int isMinimized( lua_State* L );
		
		//! isMaximized
		int isMaximized( lua_State* L );
		
		//! setCloseable
		int setCloseable( lua_State* L );
		
		//! isCloseable
		int isCloseable( lua_State* L );
		
		//! maximize
		int maximize( lua_State* L );
		
		//! unMaximize
		int unMaximize( lua_State* L );
		
		//! minimize
		int minimize( lua_State* L );
		
		//! restore
		int restore( lua_State* L );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_window>::FunctionType methods[];
		static Lunar<_lua_window>::PropertyType properties[];
	
};

#endif