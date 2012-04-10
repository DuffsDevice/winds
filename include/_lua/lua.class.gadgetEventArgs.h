// Check if already included
#ifndef _WIN_LUAGADGETEVENTARGS_
#define _WIN_LUAGADGETEVENTARGS_

extern "C"{
	#include "_lua/lua.h"
	#include "_lua/lauxlib.h"
	#include "_lua/lualib.h"
}

#include <utility>
#include "_lua/lunar.h"
#include "_type/type.gadget.h"

extern bool luaL_is( lua_State* L , int narg , string type );

#define LUNAR_DECLARE_ALL_METHODS_OF(g) \
	LUNAR_DECLARE_METHOD(g,bubbleEvent), \
	LUNAR_DECLARE_METHOD(g,bubbleRefresh), \
	LUNAR_DECLARE_METHOD(g,refreshBitmap), \
	LUNAR_DECLARE_METHOD(g,getBitmap), \
	LUNAR_DECLARE_METHOD(g,getWindows), \
	LUNAR_DECLARE_METHOD(g,registerEventHandler), \
	LUNAR_DECLARE_METHOD(g,unregisterEventHandler), \
	LUNAR_DECLARE_METHOD(g,generateEvent), \
	LUNAR_DECLARE_METHOD(g,triggerEvent), \
	LUNAR_DECLARE_METHOD(g,canReactTo), \
	LUNAR_DECLARE_METHOD(g,handleEvent), \
	LUNAR_DECLARE_METHOD(g,handleEventDefault), \
	LUNAR_DECLARE_METHOD(g,getAbsoluteX), \
	LUNAR_DECLARE_METHOD(g,getAbsoluteY), \
	LUNAR_DECLARE_METHOD(g,getX), \
	LUNAR_DECLARE_METHOD(g,getY), \
	LUNAR_DECLARE_METHOD(g,setX), \
	LUNAR_DECLARE_METHOD(g,setY), \
	LUNAR_DECLARE_METHOD(g,moveTo), \
	LUNAR_DECLARE_METHOD(g,moveRelative), \
	LUNAR_DECLARE_METHOD(g,getParent), \
	LUNAR_DECLARE_METHOD(g,setParent), \
	LUNAR_DECLARE_METHOD(g,removeChild), \
	LUNAR_DECLARE_METHOD(g,addChild), \
	LUNAR_DECLARE_METHOD(g,getDimensions), \
	LUNAR_DECLARE_METHOD(g,getAbsoluteDimensions), \
	LUNAR_DECLARE_METHOD(g,getHeight), \
	LUNAR_DECLARE_METHOD(g,setHeight), \
	LUNAR_DECLARE_METHOD(g,getWidth), \
	LUNAR_DECLARE_METHOD(g,setWidth), \
	LUNAR_DECLARE_METHOD(g,toDerived) \


/**
 * Proxy Classes
**/
class _lua_gadget{
	
	private:
		
		_lua_gadget* getLuaGadget( lua_State* L , int narg );
	
	public:
		
		_gadget* gadget;
		
		// Ctor
		_lua_gadget( _gadget* w );
		
		//! Lua-Dtor
		~_lua_gadget();
		
		// Lua-Ctor
		_lua_gadget( lua_State* L );
		
		/**
		 * Lua-GaggetEventargs
		**/
		class _lua_gadgetEventArgs : public _gadgetEventArgs {	
			
			public:
				
				//! Ctor...
				_lua_gadgetEventArgs( _gadgetEventArgs gea );
				
				//! Lua-Ctor...
				_lua_gadgetEventArgs( lua_State* L );
				
				//! reset
				int reset(lua_State* L);
				
				//! getDestination
				int getDestination(lua_State* L);
				
				//! getSource
				int getSource(lua_State* L);
				
				//! setDestination
				int setDestination(lua_State* L);
				
				//! setSource
				int setSource(lua_State* L);
				
				//! Set X
				int setPosX(lua_State* L);
				
				//! Set Y
				int setPosY(lua_State* L);
				
				//! Get X
				int getPosX(lua_State* L);
				
				//! Get Y
				int getPosY(lua_State* L);
				
				//! Set X
				int setDeltaX(lua_State* L);
				
				//! Set Y
				int setDeltaY(lua_State* L);
				
				//! Get X
				int getDeltaX(lua_State* L);
				
				//! Get Y
				int getDeltaY(lua_State* L);
				
				//! Get Key-Code
				int getKeyCode(lua_State* L);
				
				//! Set Key-Code
				int setKeyCode(lua_State* L);
				
				//! Get Held Time of the key that triggered the Event
				int getHeldTime(lua_State* L);
				
				//! Set Held Time of the key that triggered the Event
				int setHeldTime(lua_State* L);
				
				//! Get KeyCode State of that Moment the Event was triggered
				int getCurrentKeyCodes(lua_State* L);
				
				//! Set KeyCode State of that Moment the Event was triggered
				int setCurrentKeyCodes(lua_State* L);
				
				//! Set Damaged Rects(The Rects that have to be repainted by the parent)
				int setDamagedRects(lua_State* L);
				
				//! Get Damaged Rects(The Rects that have to be repainted by the parent)
				int getDamagedRects(lua_State* L);
				
				//! Check if event is a bubble-Refresh-One
				int isBubbleRefresh(lua_State* L);
				
				//! Check if event is a bubble-Refresh-One
				int setBubbleRefresh(lua_State* L);
				
				
				//! Lunar !//
				static const char className[];
				static Lunar<_lua_gadgetEventArgs>::RegType methods[];
			
		};
		
		class _lua_gadgetEvent : public _gadgetEvent {
			
			public:
				
				// Ctor...
				_lua_gadgetEvent( _gadgetEvent e );
				
				// Lua-Ctor...
				_lua_gadgetEvent( lua_State* L );
				
				//! setType
				int setType( lua_State* L );
				
				//! getType
				int getType( lua_State* L );
				
				//! getArgs
				int getArgs( lua_State* L );
				
				//! setArgs
				int setArgs( lua_State* L );
				
				
				//! getGadget
				int getGadget( lua_State* L );
				
				
				//! Lunar !//
				static const char className[];
				static Lunar<_lua_gadgetEvent>::RegType methods[];
		};
		
		//! bubbleEvent
		int bubbleEvent(lua_State* L);
		
		//! bubbleRefresh
		int bubbleRefresh(lua_State* L);
		
		//! refreshBitmap
		int refreshBitmap( lua_State* );
		
		
		//! getBitmap
		int getBitmap( lua_State* L );
		
		
		//! getWindows
		int getWindows( lua_State* L );
		
		//! registerEventHandler
		int registerEventHandler( lua_State* L );
		
		//! unregisterEventHandler
		int unregisterEventHandler( lua_State* L );
		
		//! generateEvent
		int generateEvent(lua_State* L);
		
		//! triggerEvent
		int triggerEvent(lua_State* L);
		
		//! canReactTo
		int canReactTo( lua_State* L );
		
		//! handleEvent
		int handleEvent( lua_State* L );
		
		//! handleEventDefault
		int handleEventDefault( lua_State* L );
		
		//! Get absolute X
		int getAbsoluteX(lua_State* L);
		
		//! Get absolute Y
		int getAbsoluteY(lua_State* L);
		
		//! Get X
		int getX(lua_State* L);
		
		//! Get Y
		int getY(lua_State* L);
		
		//! Set X
		int setX(lua_State* L);
		
		//! Set Y
		int setY(lua_State* L);
		
		//! moveTo
		int moveTo(lua_State* L);
		
		//! moveRelative
		int moveRelative(lua_State* L);
		
		//! getParent
		int getParent( lua_State* L );
		
		//! setParent
		int setParent( lua_State* L );
		
		//! removeChild
		int removeChild( lua_State* L );
		
		//! addChild
		int addChild( lua_State* L );
		
		//! getDimensions
		int getDimensions( lua_State* L );
		
		//! getAbsoluteDimensions
		int getAbsoluteDimensions( lua_State* L );
		
		//! setDimensions
		int setDimensions( lua_State* L );
		
		//! getHeight
		int getHeight( lua_State* L );
		
		//! getWidth
		int getWidth( lua_State* L );
		
		//! setWidth
		int setWidth(lua_State* L);
		
		//! setHeight
		int setHeight(lua_State* L);
		
		//! toDerived !//
		//! Method to get the lua-class of the gadget, because there is no inheritance in Lua
		int toDerived( lua_State* );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_gadget>::RegType methods[];
};

typedef _lua_gadget::_lua_gadgetEvent _lua_gadgetEvent;
typedef _lua_gadget::_lua_gadgetEventArgs _lua_gadgetEventArgs;

#endif