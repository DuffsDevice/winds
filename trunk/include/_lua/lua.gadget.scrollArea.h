// Check if already included
#ifndef _WIN_L_SCROLLAREA_
#define _WIN_L_SCROLLAREA_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.scrollArea.h"


/**
 * Proxy Classes
 */
class _lua_scrollarea : public _lua_gadget{	
	
	public:
		
		_lua_scrollarea( lua_State* L );
		
		_lua_scrollarea( _scrollArea* s ) : _lua_gadget( s , false )
		{ }
		
		//! setScrollTypeX
		int setScrollTypeX( lua_State* L );
		
		//! setScrollTypeY
		int setScrollTypeY( lua_State* L );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_scrollarea>::FunctionType methods[];
		static Lunar<_lua_scrollarea>::PropertyType properties[];
	
};

#endif