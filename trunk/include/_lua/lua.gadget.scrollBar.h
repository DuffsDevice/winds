// Check if already included
#ifndef _WIN_L_SCROLLBAR_
#define _WIN_L_SCROLLBAR_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.scrollBar.h"


/**
 * Proxy Classes
 */
class _lua_scrollbar : public _lua_gadget , public _lua_interface_input<_scrollBar> {	
	
	public:
		
		_lua_scrollbar( lua_State* L );
		
		_lua_scrollbar( _scrollBar* s ) : _lua_gadget( s , false )
		{ }
		
		//! setValue
		int setValue( lua_State* L );
		
		//! getValue
		int getValue( lua_State* L );
		
		//! setDimension
		int setDimension( lua_State* L );
		
		//! getDimension
		int getDimension( lua_State* L );
		
		//! setStep
		int setStep( lua_State* L );
		
		//! getStep
		int getStep( lua_State* L );
		
		//! setLength
		int setLength( lua_State* L );
		
		//! getLength
		int getLength( lua_State* L );
		
		//! setLength2
		int setLength2( lua_State* L );
		
		//! getLength2
		int getLength2( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_scrollbar>::FunctionType methods[];
		static Lunar<_lua_scrollbar>::PropertyType properties[];
	
};

#endif