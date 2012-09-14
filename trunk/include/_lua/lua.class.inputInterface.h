// Check if already included
#ifndef _WIN_LUA_INPUTINTERFACE_
#define _WIN_LUA_INPUTINTERFACE_

extern "C"{
	#include "_lua/lua.h"
	#include "_lua/lauxlib.h"
	#include "_lua/lualib.h"
}
#include "_lua/lunar.h"
#include "_type/type.gadget.h"
#include "interface.input.h"

/**
 * Proxy Classes
**/
class _lua_interface_input{	
	
	public:
		
		_interface_input*	input;
		
		_lua_interface_input( _interface_input* i );
		
		_lua_interface_input(){ input = nullptr; }
		
		virtual ~_lua_interface_input(){};
		
		//! setAlign
		int setAlign( lua_State* L );
		
		//! setVAlign
		int setVAlign( lua_State* L );
		
		//! getAlign
		int getAlign( lua_State* L );
		
		//! getVAlign
		int getVAlign( lua_State* L );
		
		//! setIntValue
		int setIntValue( lua_State* L );
		
		//! setStrValue
		int setStrValue( lua_State* L );
		
		//! getIntValue
		int getIntValue( lua_State* L );
		
		//! getStrValue
		int getStrValue( lua_State* L );
		
		//! getTitle
		int getTitle( lua_State* L );
		
		//! setTitle
		int setTitle( lua_State* L );
	
};

#endif