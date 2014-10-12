// Check if already included
#ifndef _WIN_L_PROGRESSBAR_
#define _WIN_L_PROGRESSBAR_

#include <_lua/lua.lunar.h>
#include <_lua/lua.class.gadget.h>
#include <_gadget/gadget.progressbar.h>

/**
 * Proxy Classes
 */
class _lua_progressbar : public _lua_gadget
{
	public:
		
		_lua_progressbar( lua_State* L );
		
		_lua_progressbar( _progressBar* p ) : _lua_gadget( p , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_progressbar>::FunctionType methods[];
		static Lunar<_lua_progressbar>::PropertyType properties[];
		using baseclasses = Lunar<_lua_progressbar>::BaseClassType<_lua_gadget>;
};

#endif