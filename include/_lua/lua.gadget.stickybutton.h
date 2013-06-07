// Check if already included
#ifndef _WIN_LUASTICKYBUTTON_
#define _WIN_LUASTICKYBUTTON_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.stickybutton.h"

/**
 * Proxy Classes
 */
class _lua_stickybutton : public _lua_gadget , public _lua_interface_input<_stickybutton> {
	
	public:
		
		_lua_stickybutton( lua_State* L );
		
		_lua_stickybutton( _stickybutton* c ) : _lua_gadget( c ) , _lua_interface_input( c )
		{ }
		
		//! setAutoSelect
		int setAutoSelect( lua_State* L );
		
		//! isAutoSelect
		int isAutoSelect( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_stickybutton>::FunctionType methods[];
		static Lunar<_lua_stickybutton>::PropertyType properties[];
	
};

#endif