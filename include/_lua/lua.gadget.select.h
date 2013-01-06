// Check if already included
#ifndef _WIN_LUASELECT_
#define _WIN_LUASELECT_

#include "_lua/lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_lua/lua.class.inputInterface.h"
#include "_gadget/gadget.select.h"

/**
 * Proxy Classes
**/
class _lua_select : public _lua_gadget , public _lua_interface_input<_select>
{	
	
	public:
		
		_lua_select( lua_State* L );
		
		_lua_select( _select* b ) : _lua_gadget( b ) , _lua_interface_input( b ) { }
		
		//! addIndex
		int addIndex( lua_State* L );
		
		//! removeIndex
		int removeIndex( lua_State* L );
		
		//! clearList
		int clearList( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_select>::FunctionType methods[];
		static Lunar<_lua_select>::PropertyType properties[];
	
};

#endif