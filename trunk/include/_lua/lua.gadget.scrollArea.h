// Check if already included
#ifndef _WIN_LUASCROLLAREA_
#define _WIN_LUASCROLLAREA_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.scrollArea.h"


/**
 * Proxy Classes
**/
class _lua_scrollArea : public _lua_gadget{	
	
	public:
		
		_lua_scrollArea( lua_State* L );
		
		_lua_scrollArea( _scrollArea* s ) : _lua_gadget( s )
		{ }
		
		//! setScrollTypeX
		int setScrollTypeX( lua_State* L );
		
		//! setScrollTypeY
		int setScrollTypeY( lua_State* L );
		
		//! setCanvasWidth
		int setCanvasWidth( lua_State* L );
		
		//! setCanvasHeight
		int setCanvasHeight( lua_State* L );
		
		//! scrollToX
		int scrollToX( lua_State* L );
		
		//! scrollToY
		int scrollToY( lua_State* L );
		
		//! scrollTo
		int scrollTo( lua_State* L );
		
		//! getScrollY
		int getScrollY( lua_State* L );
		
		//! getScrollX
		int getScrollX( lua_State* L );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_scrollArea>::FunctionType methods[];
		static Lunar<_lua_scrollArea>::PropertyType properties[];
	
};

#endif