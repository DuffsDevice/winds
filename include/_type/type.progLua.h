// Check if already included
#ifndef _WIN_T_LUAPROG_
#define _WIN_T_LUAPROG_

//! Lua
#include "_lua/lua.hpp"

//! Program
#include "_type/type.program.h"

class _progLua : public _program
{
	
	private:
		
		//! The lua Interpreter state
		lua_State* 				state;
		
		//! Array filled with the methods that will be registered
		static luaL_Reg windowsLibrary[];
		
		//! VBL-function to collect garbage and reset lua_stack
		void 		collector();
		
		//! Lua functions
		static int	lua_getLocalizedString( lua_State* L );
		static int	lua_getCurrentFocus( lua_State* L );
		static int	lua_addChild( lua_State* L );
		static int	lua_executeTimer( lua_State* L );
		static int	lua_terminateTimer( lua_State* L );
		static int	lua_readRegistryIndex( lua_State* L );
		static int	lua_writeRegistryIndex( lua_State* L );
		static int	lua_deleteRegistryIndex( lua_State* L );
		static int	lua_deleteRegistrySection( lua_State* L );
		static int	lua_RGB( lua_State* L );
		static int	lua_RGBA( lua_State* L );
		static int	lua_RGB_GETR( lua_State* L );
		static int	lua_RGB_GETG( lua_State* L );
		static int	lua_RGB_GETB( lua_State* L );
		static int	lua_RGB_GETA( lua_State* L );
		
		static int	lua_keyboardIsRegistered( lua_State* L );
		static int	lua_keyboardIsOpened( lua_State* L );
		static int	lua_keyboardOpen( lua_State* L );
		static int	lua_keyboardClose( lua_State* L );
		
		//! system.exit() method
		static int	lua_exit( lua_State* L );
		
		//! Register DSWindows-Libs
		static int	lua_requirePackage( lua_State* L );
		
	public:
		
		//! Ctor
		_progLua( string prog );
		
		//! Dtor
		~_progLua();
		
		void main( _cmdArgs& args );
};

#endif