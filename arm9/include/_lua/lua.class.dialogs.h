// Check if already included
#ifndef _WIN_L_DIALOGS_
#define _WIN_L_DIALOGS_

#include "_lua/lua.lunar.h"
#include "_dialog/dialog.yesno.h"
#include "_dialog/dialog.entertext.h"
#include "_dialog/dialog.errorwarninginfo.h"
#include "_dialog/dialog.image.h"
#include "_dialog/dialog.file.save.h"

/**
 * Proxy Classes
 */
class _lua_yesnodialog : public _yesNoDialog
{
	public:
		
		//! Constructor
		_lua_yesnodialog( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_yesnodialog>::FunctionType methods[];
		static Lunar<_lua_yesnodialog>::PropertyType properties[];
};

class _lua_entertextdialog : public _enterTextDialog
{
	public:
		
		//! Constructor
		_lua_entertextdialog( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_entertextdialog>::FunctionType methods[];
		static Lunar<_lua_entertextdialog>::PropertyType properties[];
};

class _lua_imagedialog : public _imageDialog
{
	public:
		
		//! Constructor
		_lua_imagedialog( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_imagedialog>::FunctionType methods[];
		static Lunar<_lua_imagedialog>::PropertyType properties[];
};

class _lua_errordialog : public _errorDialog
{
	public:
		
		//! Constructor
		_lua_errordialog( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_errordialog>::FunctionType methods[];
		static Lunar<_lua_errordialog>::PropertyType properties[];
};

class _lua_infodialog : public _infoDialog
{
	public:
		
		//! Constructor
		_lua_infodialog( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_infodialog>::FunctionType methods[];
		static Lunar<_lua_infodialog>::PropertyType properties[];
};

class _lua_warningdialog : public _warningDialog
{
	public:
		
		//! Constructor
		_lua_warningdialog( lua_State* L );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_warningdialog>::FunctionType methods[];
		static Lunar<_lua_warningdialog>::PropertyType properties[];
};

class _lua_filesavedialog : public _fileSaveDialog
{
	public:
		
		//! Constructor
		_lua_filesavedialog( lua_State* L );
		
		//! getDirentry
		int getDirentry( lua_State* );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_filesavedialog>::FunctionType methods[];
		static Lunar<_lua_filesavedialog>::PropertyType properties[];
};

#endif