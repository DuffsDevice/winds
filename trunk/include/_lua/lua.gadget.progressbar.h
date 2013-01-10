// Check if already included
#ifndef _WIN_LUAPROGRESSBAR_
#define _WIN_LUAPROGRESSBAR_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.progressbar.h"

/**
 * Proxy Classes
**/
class _lua_progressbar : public _lua_gadget , public _lua_interface_input<_progressbar> {
	
	public:
		
		_lua_progressbar( lua_State* L );
		
		_lua_progressbar( _progressbar* p ) : _lua_gadget( p ) , _lua_interface_input( p )
		{ }
		
		//! setBarType
		int setBarType( lua_State* L );
		
		//! getBarType
		int getBarType( lua_State* L );
		
		//! setColorScheme
		int setColorScheme( lua_State* L );
		
		//! getColorScheme
		int getColorScheme( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_progressbar>::FunctionType methods[];
		static Lunar<_lua_progressbar>::PropertyType properties[];
	
};

#endif