// Check if already included
#ifndef _WIN_L_ANIMATION_
#define _WIN_L_ANIMATION_

#include <_lua/lua.lunar.h>
#include <_type/type.h>
#include <_type/type.animation.h>

/**
 * Proxy Classes
 */
class _lua_animation : public _animation
{
	public:
		
		_lua_animation( _animation a );
		
		//! Constructor
		_lua_animation( lua_State* L );
		
		//! setEasing
		int setEasing(lua_State* L);
		
		//! getEasing
		int getEasing(lua_State* L);
		
		//! terminate
		int terminate(lua_State* L);
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_animation>::FunctionType methods[];
		static Lunar<_lua_animation>::PropertyType properties[];
	
};

#endif