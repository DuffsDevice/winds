// Check if already included
#ifndef _WIN_L_EVENT_
#define _WIN_L_EVENT_

#include "_lua/lua.lunar.h"
#include "_type/type.event.h"

class _lua_event : public _event {
	
	public:
		
		// Ctor...
		_lua_event( _event e );
		
		// Lua-Ctor...
		_lua_event( lua_State* L );
		
		//! setType
		int setType( lua_State* L );
		
		//! getType
		int getType( lua_State* L );
		
		//! getGadget
		int getGadget( lua_State* L );
		
		
		
		int resetParams(lua_State* L);//! resetParams
		int getDestination(lua_State* L);//! getDestination
		int setDestination(lua_State* L);//! setDestination
		int setPressure(lua_State* L);//! Set Pressure
		int getPressure(lua_State* L);//! Get Pressure
		int setPosX(lua_State* L);//! Set X
		int setPosY(lua_State* L);//! Set Y
		int getPosX(lua_State* L);//! Get X
		int getPosY(lua_State* L);//! Get Y
		int setDeltaX(lua_State* L);//! Set Delta X
		int setDeltaY(lua_State* L);//! Set Delta Y
		int getDeltaX(lua_State* L);//! Get Delta X
		int getDeltaY(lua_State* L);//! Get Delta Y
		int getKeyCode(lua_State* L);//! Get Key-Code
		int setKeyCode(lua_State* L);//! Set Key-Code
		int getHeldTime(lua_State* L);//! Get Held Time of the key that triggered the Event
		int setHeldTime(lua_State* L);//! Set Held Time of the key that triggered the Event
		int getCurrentKeys(lua_State* L);//! Get KeyCode State of that Moment the Event was triggered
		int setCurrentKeys(lua_State* L);//! Set KeyCode State of that Moment the Event was triggered
		int setDamagedRects(lua_State* L);//! Set Damaged Rects(The Rects that have to be repainted by the parent)
		int getDamagedRects(lua_State* L);//! Get Damaged Rects(The Rects that have to be repainted by the parent)
		int hasClippingRects(lua_State* L);//! Check if event has Clipping Rects
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_event>::FunctionType methods[];
		static Lunar<_lua_event>::PropertyType properties[];
};

#endif