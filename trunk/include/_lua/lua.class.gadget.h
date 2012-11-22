// Check if already included
#ifndef _WIN_LUAGADGET_
#define _WIN_LUAGADGET_

#include "_lua/lunar.h"
#include "_type/type.gadget.h"

#define GADGET_FUNCS( g ) \
	{"delete",&g::_delete}, \
	LUA_CLASS_FUNC(g,bubbleEvent), \
	LUA_CLASS_FUNC(g,bubbleRefresh), \
	LUA_CLASS_FUNC(g,refreshBitmap), \
	LUA_CLASS_FUNC(g,getBitmap), \
	LUA_CLASS_FUNC(g,getBitmapPort), \
	LUA_CLASS_FUNC(g,getScreen), \
	LUA_CLASS_FUNC(g,hasFocus), \
	LUA_CLASS_FUNC(g,registerEventHandler), \
	LUA_CLASS_FUNC(g,unregisterEventHandler), \
	LUA_CLASS_FUNC(g,generateEvent), \
	LUA_CLASS_FUNC(g,triggerEvent), \
	LUA_CLASS_FUNC(g,canReactTo), \
	LUA_CLASS_FUNC(g,handleEvent), \
	LUA_CLASS_FUNC(g,handleEventDefault), \
	LUA_CLASS_FUNC(g,getAbsoluteX), \
	LUA_CLASS_FUNC(g,getAbsoluteY), \
	LUA_CLASS_FUNC(g,removeChild), \
	LUA_CLASS_FUNC(g,removeChildren), \
	LUA_CLASS_FUNC(g,addChild), \
	LUA_CLASS_FUNC(g,addEnhancedChild), \
	LUA_CLASS_FUNC(g,getAbsoluteDimensions), \
	LUA_CLASS_FUNC(g,getSize), \
	LUA_CLASS_FUNC(g,moveTo), \
	LUA_CLASS_FUNC(g,moveRelative), \
	LUA_CLASS_FUNC(g,toDerived), \
	LUA_CLASS_FUNC(g,enhanceToParent), \
	LUA_CLASS_FUNC(g,isEnhanced) \
	
#define GADGET_ATTRS(g) \
	LUA_CLASS_ATTR(g,Padding,"padding"), \
	LUA_CLASS_ATTR(g,X,"x"), \
	LUA_CLASS_ATTR(g,Y,"y"), \
	LUA_CLASS_ATTR(g,Parent,"parent"), \
	LUA_CLASS_ATTR(g,Dimensions,"dimensions"), \
	LUA_CLASS_ATTR(g,Height,"height"), \
	LUA_CLASS_ATTR(g,Width,"width"), \
	LUA_CLASS_ATTR_RD(g,Type,"type") \


/**
 * Proxy Classes
**/
class _lua_gadget{
	
	private:
		
		_lua_gadget* getLuaGadget( lua_State* L , int narg );
		
		_lua_gadget* pushLuaGadget( lua_State* L , _lua_gadget* g );
		
	public:
		
		_gadget* gadget;
		
		// Implicit Ctor
		void setGadget( _gadget* g );
		
		// Lua-Ctor
		_lua_gadget(){}
		
		// Lua-Ctor
		_lua_gadget( _gadget* g );
		
		// Lua-Ctor
		_lua_gadget( lua_State* L );
		
		//! Lua-Dtor
		virtual ~_lua_gadget();
		
		//! Lua-Dtor
		int _delete( lua_State* L);
		
		//! bubbleEvent
		int bubbleEvent(lua_State* L);
		
		//! bubbleRefresh
		int bubbleRefresh(lua_State* L);
		
		//! refreshBitmap
		int refreshBitmap( lua_State* );
		
		//! hasFocus
		int hasFocus( lua_State* );
		
		//! getBitmap
		int getBitmap( lua_State* L );
		
		//! getBitmapPort
		int getBitmapPort( lua_State* L );
		
		//! getScreen
		int getScreen( lua_State* L );
		
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
		
		//! enhanceToParent
		int enhanceToParent( lua_State* L );
		
		//! removeChild
		int removeChild( lua_State* L );
		
		//! removeChildren
		int removeChildren( lua_State* L );
		
		//! addChild
		int addChild( lua_State* L );
		
		//! addEnhancedChild
		int addEnhancedChild( lua_State* L );
		
		//! getDimensions
		int getDimensions( lua_State* L );
		
		//! getAbsoluteDimensions
		int getAbsoluteDimensions( lua_State* L );
		
		//! setDimensions
		int setDimensions( lua_State* L );
		
		//! setPadding
		int setPadding( lua_State* L );
		
		//! getPadding
		int getPadding( lua_State* L );
		
		//! isEnhanced
		int isEnhanced( lua_State* L );
		
		//! getHeight
		int getHeight( lua_State* L );
		
		//! getWidth
		int getWidth( lua_State* L );
		
		//! setWidth
		int setWidth(lua_State* L);
		
		//! setHeight
		int setHeight(lua_State* L);
		
		//! getSize
		int getSize( lua_State* L );
		
		//! getType
		int getType( lua_State* L );
		
		//! toDerived !//
		//! Method to get the lua-class of the gadget, because there is no inheritance in Lua
		int toDerived( lua_State* );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_gadget>::FunctionType methods[];
		static Lunar<_lua_gadget>::PropertyType properties[];
};

#endif