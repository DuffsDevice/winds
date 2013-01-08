// Check if already included
#ifndef _WIN_LUA_INPUTINTERFACE_
#define _WIN_LUA_INPUTINTERFACE_

#include "_lua/lunar.h"

/**
 * Proxy Classes
**/
template<typename T>
class _lua_interface_input{
	
	public:
		
		T*	input;
		
		virtual ~_lua_interface_input(){};
		
		_lua_interface_input( T* i ) : 
			input( i )
		{ }
		
		_lua_interface_input(){ input = nullptr; }
		
		//! setAlign
		int setAlign( lua_State* L ){ this->input->setAlign( string2align[ luaL_checkstring( L , -1 ) ] ); return 0; };

		//! setVAlign
		int setVAlign( lua_State* L ){ this->input->setVAlign( string2valign[ luaL_checkstring( L , -1 ) ] ); return 0; };

		//! getAlign
		int getAlign( lua_State* L ) { lua_pushstring( L , align2string[ this->input->getAlign() ].c_str() ); return 1; }

		//! getVAlign
		int getVAlign( lua_State* L ) { lua_pushstring( L , valign2string[ this->input->getVAlign() ].c_str() ); return 1; }

		//! setIntValue
		int setIntValue( lua_State* L ){ this->input->setIntValue( luaL_checkint( L , -1 ) ); return 0; };

		//! setStrValue
		int setStrValue( lua_State* L ){ this->input->setStrValue( luaL_checkstring( L , -1 ) ); return 0; };

		//! setTitle
		int setTitle( lua_State* L ){ this->input->setTitle( luaL_checkstring( L , -1 ) ); return 0; };

		//! getIntValue
		int getIntValue( lua_State* L ){ lua_pushnumber( L , this->input->getIntValue() ); return 1; }

		//! getStrValue
		int getStrValue( lua_State* L ){ lua_pushstring( L , this->input->getStrValue().c_str() ); return 1; }

		//! getTitle
		int getTitle( lua_State* L ){ lua_pushstring( L , this->input->getTitle().c_str() ); return 1; }
};

#endif