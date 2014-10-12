// Check if already included
#ifndef _WIN_L_TASKINFO_
#define _WIN_L_TASKINFO_

#include <_lua/lua.lunar.h>
#include <_lua/lua.class.gadget.h>
#include <_gadget/gadget.taskinfo.h>

/**
 * Proxy Classes
 */
class _lua_taskinfo : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_taskinfo( lua_State* L );
		
		//! C-Ctor
		_lua_taskinfo( _taskInfo* tI ) : _lua_gadget( tI , false )
		{ }
		
		//! displayNotification
		int displayNotification( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_taskinfo>::FunctionType methods[];
		static Lunar<_lua_taskinfo>::PropertyType properties[];
		using baseclasses = Lunar<_lua_taskinfo>::BaseClassType<_lua_gadget>;
};

#endif