// Check if already included
#ifndef _WIN_T_LUAPROG_
#define _WIN_T_LUAPROG_

#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <time.h>

//! Lua
extern "C"{
#include "_lua/lua.h"
#include "_lua/lauxlib.h"
#include "_lua/lualib.h"
}

#include "_type/type.program.h"

using namespace std;


class _progLua : public _program
{
	
	private:
		
		static const luaL_reg 	library[];
		
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