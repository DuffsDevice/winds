#ifndef _WIN_L_POPUP_
#define _WIN_L_POPUP_

#include <_lua/lua.lunar.h>
#include <_lua/lua.class.gadget.h>
#include <_gadget/gadget.popup.h>

/**
 * Proxy Classes
 */
class _lua_popup : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_popup( lua_State* L );
		
		//! C-Ctor
		_lua_popup( _popup* p ) : _lua_gadget( p , false )
		{ }
		
		//! show
		int show( lua_State* L );
		
		//! showCentered
		int showCentered( lua_State* L );
		
		//! shelve
		int shelve( lua_State* L );
		
		//! toggle
		int toggle( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_popup>::FunctionType	methods[];
		static Lunar<_lua_popup>::PropertyType	properties[];
		using baseclasses = Lunar<_lua_popup>::BaseClassType<_lua_gadget>;
};

#endif