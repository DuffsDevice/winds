#ifndef _WIN_CONFIG_
#define _WIN_CONFIG_

/////////////////////////
// WinDS Build Version //
/////////////////////////

/**
 * WinDS-Version
 */
#define _WIN_VER_MAJOR_ 0
#define _WIN_VER_MINOR_ 98
#define _WIN_VER_STATE_ "beta"



//////////////////////
// WinDS Build Type //
//////////////////////

/**
 * Define it, if you want speed & memory profiling
 */
//#define _WIN_CONFIG_BUILD_DEBUG_PROFILING_



/////////////////////
// Lua Interpreter //
/////////////////////

/**
 * Define it, if you want the Lua interpreter to make std::pair's in Lua accessible
 * as an array using [1] and [2], additionally to .first and .second
 */
#define _WIN_CONFIG_LUA_PAIR_AS_ARRAY_


#endif