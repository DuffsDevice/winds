#include "_lua/lua.class.dialogs.h"
#include "_type/type.system.h"
#include "_lua/lua.funcs.h"
using namespace _luafunc;

/*##################################
##           Lua-Dialogs          ##
##################################*/

//! yesNoDialog
_lua_yesnodialog::_lua_yesnodialog(lua_State* L) :
	_lua_interface_dialog(
		check<string>( L , 1 )
		, check<string>( L , 2 )
		, optcheck<string>( L , 3 )
		, optcheck<string>( L , 4 )
	)
{}

//! yesNoDialog
const char _lua_yesnodialog::className[] = "YesNoDialog";


//! enterTextDialog
_lua_entertextdialog::_lua_entertextdialog(lua_State* L) :
	_lua_interface_dialog(
		check<string>( L , 1 )
		, check<string>( L , 2 )
		, optcheck<string>( L , 3 )
		, optcheck<string>( L , 4 )
	)
{}

//! enterTextDialog - Name
const char _lua_entertextdialog::className[] = "EnterTextDialog";


//! imageDialog
_lua_imagedialog::_lua_imagedialog(lua_State* L) :
	_lua_interface_dialog(
		check<string>( L , 1 )
		, check<string>( L , 2 )
		, lightcheck<_bitmap>( L , 3 , _bitmap() )
		, optcheck<string>( L , 4 )
		, optcheck<string>( L , 5 )
	)
{}

//! imageDialog - Name
const char _lua_imagedialog::className[] = "ImageDialog";


//! errorDialog
_lua_errordialog::_lua_errordialog(lua_State* L) :
	_lua_interface_dialog(
		check<string>( L , 1 )
		, optcheck<string>( L , 2 )
		, optcheck<string>( L , 3 )
	)
{}

//! errorDialog - Name
const char _lua_errordialog::className[] = "ErrorDialog";


//! infoDialog
_lua_infodialog::_lua_infodialog(lua_State* L) :
	_lua_interface_dialog(
		check<string>( L , 1 )
		, optcheck<string>( L , 2 )
		, optcheck<string>( L , 3 )
	)
{}

//! infoDialog - Name
const char _lua_infodialog::className[] = "InfoDialog";


//! warningDialog
_lua_warningdialog::_lua_warningdialog(lua_State* L) :
	_lua_interface_dialog(
		check<string>( L , 1 )
		, optcheck<string>( L , 2 )
		, optcheck<string>( L , 3 )
	)
{}

//! warningDialog - Name
const char _lua_warningdialog::className[] = "WarningDialog";