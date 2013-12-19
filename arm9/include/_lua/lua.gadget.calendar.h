// Check if already included
#ifndef _WIN_L_CALENDAR_
#define _WIN_L_CALENDAR_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.calendar.h"

/**
 * Proxy Classes
 */
class _lua_calendar : public _lua_gadget
{
	public:
		
		_lua_calendar( lua_State* L );
		
		_lua_calendar( _calendar* c ) : _lua_gadget( c , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_calendar>::FunctionType methods[];
		static Lunar<_lua_calendar>::PropertyType properties[];
	
};

#endif