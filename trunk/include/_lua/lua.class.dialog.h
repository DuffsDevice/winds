// Check if already included
#ifndef _WIN_LUADIALOG_
#define _WIN_LUADIALOG_

#include "_lua/lunar.h"
#include "_type/type.dialog.h"

/**
 * Proxy Classes
 */
class _lua_dialog : public _dialog {
	
	public:
		
		//! Constructor
		_lua_dialog( lua_State* L );
		
		//! yesNoDialog
		int yesNoDialog(lua_State* L);
		
		//! enterTextDialog
		int enterTextDialog(lua_State* L);
		
		//! imageDialog
		int imageDialog(lua_State* L);
		
		//! errorDialog
		int errorDialog(lua_State* L);
		
		//! infoDialog
		int infoDialog(lua_State* L);
		
		//! warningDialog
		int warningDialog(lua_State* L);
		
		//! execute
		int execute(lua_State* L);
		
		//! terminate
		int terminate(lua_State* L);
		
		//! getIntResult
		int getIntResult(lua_State* L);
		
		//! getStrResult
		int getStrResult(lua_State* L);
		
		//! onExit
		int onExit(lua_State* L);
		
		//! hasFinished
		int hasFinished(lua_State* L);
		
		//! wasAborted
		int wasAborted(lua_State* L);
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_dialog>::FunctionType methods[];
		static Lunar<_lua_dialog>::PropertyType properties[];
	
};

#endif