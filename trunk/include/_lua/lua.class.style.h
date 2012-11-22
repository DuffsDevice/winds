// Check if already included
#ifndef _WIN_LUASTYLE_
#define _WIN_LUASTYLE_

#include "_lua/lunar.h"
#include "_type/type.gadgetStyle.h"

/**
 * Proxy Classes
**/
class _lua_style : public _style {
		
	public:
		
		_lua_style( _style st );
		
		//! Constructor
		_lua_style( lua_State* L );
		
		//! owner
		int setOwner(lua_State* L);
		int getOwner(lua_State* L);
		
		//! data
		int setData(lua_State* L);
		int getData(lua_State* L);
		
		//! resizable
		int setResizable(lua_State* L);
		int getResizable(lua_State* L);
		
		//! destroyable
		int setDestroyable(lua_State* L);
		int getDestroyable(lua_State* L);
		
		//! focused
		int getFocused(lua_State* L);
		
		//! pressed
		int getPressed(lua_State* L);
		
		//! enhanced
		int getEnhanced(lua_State* L);
		
		//! dragged
		int getDragged(lua_State* L);
		
		//! draggable
		int setDraggable(lua_State* L);
		int getDraggable(lua_State* L);
		
		//! editable
		int setEditable(lua_State* L);
		int getEditable(lua_State* L);
		
		//! showsUpInTaskBar
		int setShowsUpInTaskBar(lua_State* L);
		int getShowsUpInTaskBar(lua_State* L);
		
		//! canTakeFocus
		int setCanTakeFocus(lua_State* L);
		int getCanTakeFocus(lua_State* L);
		
		//! canLooseFocus
		int setCanLooseFocus(lua_State* L);
		int getCanLooseFocus(lua_State* L);
		
		//! canReceiveFocus
		int setCanReceiveFocus(lua_State* L);
		int getCanReceiveFocus(lua_State* L);
		
		//! smallDragTrig
		int setSmallDragTrig(lua_State* L);
		int getSmallDragTrig(lua_State* L);
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_style>::FunctionType methods[];
		static Lunar<_lua_style>::PropertyType properties[];
	
};

#endif