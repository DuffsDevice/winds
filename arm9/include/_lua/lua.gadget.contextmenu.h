// Check if already included
#ifndef _WIN_L_CONTEXTMENU_
#define _WIN_L_CONTEXTMENU_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.contextmenu.h"

/**
 * Proxy Classes
 */
class _lua_contextmenu : public _lua_gadget
{
	public:
		
		_lua_contextmenu( lua_State* L );
		
		_lua_contextmenu( _contextMenu* s ) : _lua_gadget( s , false )
		{ }
		
		//! addIndex
		int addIndex( lua_State* L );
		
		//! removeIndex
		int removeIndex( lua_State* L );
		
		//! clearList
		int clearList( lua_State* L );
		
		//! getList
		int getList( lua_State* L );
		
		//! setList
		int setList( lua_State* L );
		
		//! getEntryFromNumber
		int getEntryFromNumber( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_contextmenu>::FunctionType methods[];
		static Lunar<_lua_contextmenu>::PropertyType properties[];
	
};

#endif