// Check if already included
#ifndef _WIN_LUASCROLLAREA_
#define _WIN_LUASCROLLAREA_

extern "C"{
	#include "_lua/lua.h"
	#include "_lua/lauxlib.h"
	#include "_lua/lualib.h"
}
#include "_lua/lua.gadget.gadgetEventArgs.h"
#include "_lua/lunar.h"
#include "_gadget/gadget.scrollArea.h"

/**
 * Proxy Classes
**/
class _lua_scrollArea : public _lua_gadget{	
	
	public:
		
		_lua_scrollArea( lua_State* L );
		
		_lua_scrollArea( _scrollArea* s ){ 
			this->setGadget( s );
		}
		
		//! setScrollTypeX
		int setScrollTypeX( lua_State* L );
		
		//! setScrollTypeY
		int setScrollTypeY( lua_State* L );
		
		//! setInnerWidth
		int setInnerWidth( lua_State* L );
		
		//! setInnerHeight
		int setInnerHeight( lua_State* L );
		
		//! scrollToX
		int scrollToX( lua_State* L );
		
		//! scrollToY
		int scrollToY( lua_State* L );
		
		//! getScrollY
		int getScrollY( lua_State* L );
		
		//! getScrollX
		int getScrollX( lua_State* L );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_scrollArea>::RegType methods[];
	
};

#endif