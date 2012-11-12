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
		
		static const luaL_Reg 	library[];
		
		static int createWindow( lua_State* L );
		
		lua_State* 				state;
		
		string 					code;
		
		// Register DSWindows-Libs
		void registerLibs();
		
	public:
		
		_progLua( string prog );
		
		~_progLua();
		
		int main( _cmdArgs& args );
		
		void init( _cmdArgs& args );
		
};

#endif