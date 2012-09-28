// Check if already included
#ifndef _WIN_LUALABEL_
#define _WIN_LUALABEL_

extern "C"{
	#include "_lua/lua.h"
	#include "_lua/lauxlib.h"
	#include "_lua/lualib.h"
}
#include "_lua/lua.gadget.gadgetEventArgs.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.label.h"
#include "_lua/lunar.h"

/**
 * Proxy Classes
**/
class _lua_label : public _lua_gadget , public _lua_interface_input {
	
	public:
		
		_lua_label( lua_State* L );
		
		_lua_label( _label* c ) : _lua_gadget( c ) , _lua_interface_input( c ) { }
		
		//! setColor
		int setColor( lua_State* L );
		
		//! setBgColor
		int setBgColor( lua_State* L );
		
		//! getColor
		int getColor( lua_State* L );
		
		//! getBgColor
		int getBgColor( lua_State* L );
		
		//! getFont
		int getFont( lua_State* L );
		
		//! getFont
		int setFont( lua_State* L );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_label>::RegType methods[];
	
};

#endif