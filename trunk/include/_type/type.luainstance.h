// Check if already included
#ifndef _WIN_LUAINSTANCE_
#define _WIN_LUAINSTANCE_

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

#include "_gadget/gadget.windows.h"

using namespace std;


class _program{
	
	private:
		
		static const luaL_reg 	library[];
		
		static int createWindow( lua_State* L );
		
		void runMain();
		
		void runInit();
		
	
	private:
		
		lua_State* 				state;
		
		string 					code;
		
		// Register DSWindows-Libs
		void registerLibs();
		
	public:
		
		_program( string prog );
		
		void run( _windows* w );
		
};

#endif