// Check if already included
#ifndef _WIN_LUAANIMATION_
#define _WIN_LUAANIMATION_

#include "_lua/lunar.h"
#include "_type/type.h"
#include "_type/type.animation.h"

/**
 * Proxy Classes
 */
class _lua_animation : public _animation {	
		
	public:
		
		_lua_animation( _animation a );
		
		//! Constructor
		_lua_animation( lua_State* L );
		
		//! setFromValue
		int setFromValue(lua_State* L);
		
		//! setToValue
		int setToValue(lua_State* L);
		
		//! setDuration
		int setDuration(lua_State* L);
		
		//! setSetter
		int setSetter(lua_State* L);
		
		//! setFinish
		int setFinish(lua_State* L);
		
		//! setEasing
		int setEasing(lua_State* L);
		
		//! getFromValue
		int getFromValue(lua_State* L);
		
		//! getToValue
		int getToValue(lua_State* L);
		
		//! getDuration
		int getDuration(lua_State* L);
		
		//! getEasing
		int getEasing(lua_State* L);
		
		//! getRunning
		int getRunning(lua_State* L);
		
		//! start
		int start(lua_State* L);
		
		//! terminate
		int terminate(lua_State* L);
		
		//! pause
		int pause(lua_State* L);
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_animation>::FunctionType methods[];
		static Lunar<_lua_animation>::PropertyType properties[];
	
};

#endif