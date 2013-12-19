// Check if already included
#ifndef _WIN_L_TEXTBOX_
#define _WIN_L_TEXTBOX_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.gadget.h"
#include "_gadget/gadget.textbox.h"

/**
 * Proxy Classes
 */
class _lua_textbox : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_textbox( lua_State* L );
		
		//! C-Ctor
		_lua_textbox( _textbox* t ) : _lua_gadget( t , false )
		{ }
		
		//! setCursor
		int setCursor( lua_State* L );
		
		//! getCursor
		int getCursor( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_textbox>::FunctionType methods[];
		static Lunar<_lua_textbox>::PropertyType properties[];
	
};

#endif