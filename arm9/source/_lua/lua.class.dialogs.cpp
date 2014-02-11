#include "_lua/lua.class.dialogs.h"
#include "_lua/lua.func.h"
#include "_lua/lua.func.wrap.h"
using namespace _luafunc;

/*##################################
##           Lua-Dialogs          ##
##################################*/

//! yesNoDialog
_lua_yesnodialog::_lua_yesnodialog(lua_State* L) :
	_yesNoDialog(
		check<string>( L , 1 )
		, check<string>( L , 2 )
		, optcheck<string>( L , 3 )
		, optcheck<string>( L , 4 )
	)
{}

//! yesNoDialog
const char _lua_yesnodialog::className[] = "YesNoDialog";
Lunar<_lua_yesnodialog>::FunctionType _lua_yesnodialog::methods[] = {
	{ "execute"			, wrap( _lua_yesnodialog , &_dialog::execute ) },
	{ "terminate"		, wrap( _lua_yesnodialog , &_dialog::terminate ) },
	{ "setCallback"		, wrap( _lua_yesnodialog , &_dialog::setCallback ) },
	{ "deleteCallback"	, wrap( _lua_yesnodialog , &_dialog::deleteCallback ) },
	LUA_CLASS_FUNC_END
};
Lunar<_lua_yesnodialog>::PropertyType _lua_yesnodialog::properties[] = {
	{ "running"	, wrap( _lua_yesnodialog , &_dialog::isRunning ) , nullptr },
	LUA_CLASS_ATTR_END
};


//! enterTextDialog
_lua_entertextdialog::_lua_entertextdialog(lua_State* L) :
	_enterTextDialog(
		check<string>( L , 1 )
		, check<string>( L , 2 )
		, optcheck<string>( L , 3 )
		, optcheck<string>( L , 4 )
	)
{}

//! enterTextDialog - Name
const char _lua_entertextdialog::className[] = "EnterTextDialog";
Lunar<_lua_entertextdialog>::FunctionType _lua_entertextdialog::methods[] = {
	{ "execute"			, wrap( _lua_entertextdialog , &_dialog::execute ) },
	{ "terminate"		, wrap( _lua_entertextdialog , &_dialog::terminate ) },
	{ "setCallback"		, wrap( _lua_entertextdialog , &_dialog::setCallback ) },
	{ "deleteCallback"	, wrap( _lua_entertextdialog , &_dialog::deleteCallback ) },
	LUA_CLASS_FUNC_END
};
Lunar<_lua_entertextdialog>::PropertyType _lua_entertextdialog::properties[] = {
	{ "running"			, wrap( _lua_entertextdialog , &_dialog::isRunning )				, nullptr },
	{ "result"			, wrap( _lua_entertextdialog , &_enterTextDialog::getResult )		, nullptr },
	{ "initialValue"	, wrap( _lua_entertextdialog , &_enterTextDialog::getInitialValue ) , wrap( _lua_entertextdialog , &_enterTextDialog::setInitialValue ) },
	LUA_CLASS_ATTR_END
};


//! imageDialog
_lua_imagedialog::_lua_imagedialog(lua_State* L) :
	_imageDialog(
		check<string>( L , 1 )
		, check<string>( L , 2 )
		, lightcheck<_bitmap>( L , 3 , _bitmap() )
		, optcheck<string>( L , 4 )
		, optcheck<string>( L , 5 )
	)
{}

//! imageDialog - Name
const char _lua_imagedialog::className[] = "ImageDialog";
Lunar<_lua_imagedialog>::FunctionType _lua_imagedialog::methods[] = {
	{ "execute"			, wrap( _lua_imagedialog , &_dialog::execute ) },
	{ "terminate"		, wrap( _lua_imagedialog , &_dialog::terminate ) },
	{ "setCallback"		, wrap( _lua_imagedialog , &_dialog::setCallback ) },
	{ "deleteCallback"	, wrap( _lua_imagedialog , &_dialog::deleteCallback ) },
	LUA_CLASS_FUNC_END
};
Lunar<_lua_imagedialog>::PropertyType _lua_imagedialog::properties[] = {
	{ "running"	, wrap( _lua_imagedialog , &_dialog::isRunning ) , nullptr },
	LUA_CLASS_ATTR_END
};


//! errorDialog
_lua_errordialog::_lua_errordialog(lua_State* L) :
	_errorDialog(
		check<string>( L , 1 )
		, optcheck<string>( L , 2 )
		, optcheck<string>( L , 3 )
	)
{}

//! errorDialog - Name
const char _lua_errordialog::className[] = "ErrorDialog";
Lunar<_lua_errordialog>::FunctionType _lua_errordialog::methods[] = {
	{ "execute"			, wrap( _lua_errordialog , &_dialog::execute ) },
	{ "terminate"		, wrap( _lua_errordialog , &_dialog::terminate ) },
	{ "setCallback"		, wrap( _lua_errordialog , &_dialog::setCallback ) },
	{ "deleteCallback"	, wrap( _lua_errordialog , &_dialog::deleteCallback ) },
	LUA_CLASS_FUNC_END
};
Lunar<_lua_errordialog>::PropertyType _lua_errordialog::properties[] = {
	{ "running"	, wrap( _lua_errordialog , &_dialog::isRunning ) , nullptr },
	LUA_CLASS_ATTR_END
};


//! infoDialog
_lua_infodialog::_lua_infodialog(lua_State* L) :
	_infoDialog(
		check<string>( L , 1 )
		, optcheck<string>( L , 2 )
		, optcheck<string>( L , 3 )
	)
{}

//! infoDialog - Name
const char _lua_infodialog::className[] = "InfoDialog";
Lunar<_lua_infodialog>::FunctionType _lua_infodialog::methods[] = {
	{ "execute"			, wrap( _lua_infodialog , &_dialog::execute ) },
	{ "terminate"		, wrap( _lua_infodialog , &_dialog::terminate ) },
	{ "setCallback"		, wrap( _lua_infodialog , &_dialog::setCallback ) },
	{ "deleteCallback"	, wrap( _lua_infodialog , &_dialog::deleteCallback ) },
	LUA_CLASS_FUNC_END
};
Lunar<_lua_infodialog>::PropertyType _lua_infodialog::properties[] = {
	{ "running"	, wrap( _lua_infodialog , &_dialog::isRunning ) , nullptr },
	LUA_CLASS_ATTR_END
};


//! warningDialog
_lua_warningdialog::_lua_warningdialog(lua_State* L) :
	_warningDialog(
		check<string>( L , 1 )
		, optcheck<string>( L , 2 )
		, optcheck<string>( L , 3 )
	)
{}

//! warningDialog - Name
const char _lua_warningdialog::className[] = "WarningDialog";
Lunar<_lua_warningdialog>::FunctionType _lua_warningdialog::methods[] = {
	{ "execute"			, wrap( _lua_warningdialog , &_dialog::execute ) },
	{ "terminate"		, wrap( _lua_warningdialog , &_dialog::terminate ) },
	{ "setCallback"		, wrap( _lua_warningdialog , &_dialog::setCallback ) },
	{ "deleteCallback"	, wrap( _lua_warningdialog , &_dialog::deleteCallback ) },
	LUA_CLASS_FUNC_END
};
Lunar<_lua_warningdialog>::PropertyType _lua_warningdialog::properties[] = {
	{ "running"	, wrap( _lua_warningdialog , &_dialog::isRunning ) , nullptr },
	LUA_CLASS_ATTR_END
};