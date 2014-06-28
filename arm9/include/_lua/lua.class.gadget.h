// Check if already included
#ifndef _WIN_L_GADGET_
#define _WIN_L_GADGET_

#include "_lua/lua.lunar.h"
#include "_type/type.gadget.h"


/**
 * Proxy Classes
 */
class _lua_gadget{
	
	private:
	
		_gadget* gadget;
		bool wasAllocated;
		
	public:
		
		//! C-Ctor
		_lua_gadget( _gadget* g = nullptr , bool wasAllocated = true );
		
		//! Lua-Ctor
		_lua_gadget( lua_State* L );
		
		//! Dtor
		virtual ~_lua_gadget();
		
		
		//! Set Gadget to contain
		void setGadget( _gadget* g , bool wasAllocated = true );
		
		//! Get underlying _gadget instance
		_gadget* getGadget(){ return this->gadget; }
		template<typename T>
		T* getGadget() const {
			unused typedef typename T::_gadget def; // Tests if the supplied param is a subclass of _gadget
			return static_cast<T*>( this->gadget );
		}
		
		//! Get the internal _gadget pointer casted to a derived class of _gadget or _gadget itself
		operator _gadget&(){ return *this->gadget; }
		template<typename T>
		operator T&()
		{
			unused typedef typename T::_gadget def;
			return * static_cast<T*>(this->gadget);
		}
		
		//! applyStyle
		int applyStyle( lua_State* );
		
		//! getBitmapPort
		int getBitmapPort( lua_State* L );
		
		//! Set X
		int setX(lua_State* L);
		
		//! Set Y
		int setY(lua_State* L);
		
		//! Set Position
		int moveTo(lua_State* L);
		
		//! Move Relatively
		int moveRelative(lua_State* L);
		
		//! setWidth
		int setWidth(lua_State* L);
		
		//! setHeight
		int setHeight(lua_State* L);
		
		//! setMinWidth
		int setMinWidth(lua_State* L);
		
		//! setMinHeight
		int setMinHeight(lua_State* L);
		
		//! triggerEvent
		int triggerEvent(lua_State* L);
		
		//! getType
		int getType( lua_State* L );
		
		//! equals
		int equals( lua_State* L );
		
		//! addChild
		int addChild( lua_State* L );
		
		//! addEnhancedChild
		int addEnhancedChild( lua_State* L );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_gadget>::FunctionType methods[];
		static Lunar<_lua_gadget>::PropertyType properties[];
};

#endif