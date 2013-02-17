// Check if already included
#ifndef _WIN_LUACOUNTER_
#define _WIN_LUACOUNTER_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.counter.h"

/**
 * Proxy Classes
 */
class _lua_counter : public _lua_gadget , public _lua_interface_input<_counter> {
	
	public:
		
		_lua_counter( lua_State* L );
		
		_lua_counter( _counter* c ) : _lua_gadget( c ) , _lua_interface_input( c )
		{ }
		
		//! setUpperBound
		int setUpperBound( lua_State* L );
		
		//! setLowerBound
		int setLowerBound( lua_State* L );
		
		//! getLowerBound
		int getLowerBound( lua_State* L );
		
		//! getUpperBound
		int getUpperBound( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_counter>::FunctionType methods[];
		static Lunar<_lua_counter>::PropertyType properties[];
	
};

#endif