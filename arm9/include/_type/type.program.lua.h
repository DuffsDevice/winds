// Check if already included
#ifndef _WIN_T_LUAPROG_
#define _WIN_T_LUAPROG_

//! Lua
#include "_lua/lua.h"

//! Program
#include "_type/type.program.h"

class _progLua : public _program
{
	
	private:
		
		//! The lua Interpreter state
		lua_State* 				state;
		string*					content;
		bool					headParsed;
		
		//! Array filled with the methods that will be registered
		static luaL_Reg			windowsLibrary[];
		
		//! VBL-function to collect garbage and reset lua_stack
		void 		internalVbl();
		
		//! Lua functions
		static int	lua_getLocalizedString( lua_State* L );
		static int	lua_getCurrentFocus( lua_State* L );
		static int	lua_addChild( lua_State* L );
		static int	lua_getRegistry( lua_State* L );
		static int	lua_getFont( lua_State* L );
		static int	lua_fontChangePhrase( lua_State* L );
		static int	lua_colorChangePhrase( lua_State* L );
		static int	lua_sizeChangePhrase( lua_State* L );
		
		static int	lua_keyboardIsRegistered( lua_State* L );
		static int	lua_keyboardIsOpened( lua_State* L );
		static int	lua_keyboardOpen( lua_State* L );
		static int	lua_keyboardClose( lua_State* L );
		static int	lua_writeDebug( lua_State* L );
		static int	lua_pushEvent( lua_State* L );
		
		//! system.exit() method
		static int	lua_exit( lua_State* L );
		
		//! Register DSWindows-Libs
		static int	lua_usingClass( lua_State* L );
		
		//! registers system.* namespace
		void registerSystem();
		
		//! Parses the program header
		bool parseProgramHeader();
		
	public:
		
		//! Ctor
		_progLua( string prog );
		
		//! Main function that will be called at the start of execution (one shot)
		void internalMain( _programArgs args );
		
		//! Cleans up the Lua program and state
		void internalCleanUp();
};

#endif