// Check if already included
#ifndef _WIN_L_DIALOGS_
#define _WIN_L_DIALOGS_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.dialogInterface.h"
#include "_dialog/dialog.yesno.h"
#include "_dialog/dialog.entertext.h"
#include "_dialog/dialog.errorwarninginfo.h"
#include "_dialog/dialog.image.h"

/**
 * Proxy Classes
 */
class _lua_yesnodialog : public _lua_interface_dialog<_yesNoDialog>
{
	public:
		
		//! Constructor
		_lua_yesnodialog( lua_State* L );
		
		//! Lunar !//
		static const char className[];
	
};

class _lua_entertextdialog : public _lua_interface_dialog<_enterTextDialog>
{
	public:
		
		//! Constructor
		_lua_entertextdialog( lua_State* L );
		
		//! Lunar !//
		static const char className[];
	
};

class _lua_imagedialog : public _lua_interface_dialog<_imageDialog>
{
	public:
		
		//! Constructor
		_lua_imagedialog( lua_State* L );
		
		//! Lunar !//
		static const char className[];
	
};

class _lua_errordialog : public _lua_interface_dialog<_errorDialog>
{
	public:
		
		//! Constructor
		_lua_errordialog( lua_State* L );
		
		//! Lunar !//
		static const char className[];
	
};

class _lua_infodialog : public _lua_interface_dialog<_infoDialog>
{
	public:
		
		//! Constructor
		_lua_infodialog( lua_State* L );
		
		//! Lunar !//
		static const char className[];
	
};

class _lua_warningdialog : public _lua_interface_dialog<_warningDialog>
{
	public:
		
		//! Constructor
		_lua_warningdialog( lua_State* L );
		
		//! Lunar !//
		static const char className[];
	
};

#endif