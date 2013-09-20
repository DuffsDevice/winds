// Check if already included
#ifndef _WIN_L_COUNTER_
#define _WIN_L_COUNTER_

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
		
		_lua_counter( _counter* c ) : _lua_gadget( c , false ) , _lua_interface_input( c )
		{ }
		
		//! setUpperBound
		int setUpperBound( lua_State* L );
		
		//! setLowerBound
		int setLowerBound( lua_State* L );
		
		//! getLowerBound
		int getLowerBound( lua_State* L );
		
		//! getUpperBound
		int getUpperBound( lua_State* L );
		
		//! increase
		int increase( lua_State* L );
		
		//! decrease
		int decrease( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_counter>::FunctionType methods[];
		static Lunar<_lua_counter>::PropertyType properties[];
	
};

#endif