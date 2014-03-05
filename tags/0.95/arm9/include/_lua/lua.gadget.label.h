// Check if already included
#ifndef _WIN_L_LABEL_
#define _WIN_L_LABEL_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.label.h"

/**
 * Proxy Classes
 */
class _lua_label : public _lua_gadget
{
	public:
		
		_lua_label( lua_State* L );
		
		_lua_label( _label* c ) : _lua_gadget( c , false )
		{ }
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_label>::FunctionType methods[];
		static Lunar<_lua_label>::PropertyType properties[];
		using baseclasses = Lunar<_lua_label>::BaseClassType<_lua_gadget>;
};

#endif