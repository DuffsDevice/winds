// Check if already included
#ifndef _WIN_L_SLIDER_
#define _WIN_L_SLIDER_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.slider.h"

/**
 * Proxy Classes
 */
class _lua_slider : public _lua_gadget , public _lua_interface_input<_slider> {
	
	public:
		
		_lua_slider( lua_State* L );
		
		_lua_slider( _slider* c ) : _lua_gadget( c , false )
		{ }
		
		//! setUpperBound
		int setUpperBound( lua_State* L );
		
		//! setLowerBound
		int setLowerBound( lua_State* L );
		
		//! getLowerBound
		int getLowerBound( lua_State* L );
		
		//! getUpperBound
		int getUpperBound( lua_State* L );
		
		//! setSnap
		int setSnap( lua_State* L );
		
		//! getSnap
		int getSnap( lua_State* L );
		
		//! setLowerLabel
		int setLowerLabel( lua_State* L );
		
		//! setUpperLabel
		int setUpperLabel( lua_State* L );
		
		//! getLowerLabel
		int getLowerLabel( lua_State* L );
		
		//! getUpperLabel
		int getUpperLabel( lua_State* L );
		
		//! increase
		int increase( lua_State* L );
		
		//! decrease
		int decrease( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_slider>::FunctionType methods[];
		static Lunar<_lua_slider>::PropertyType properties[];
	
};

#endif