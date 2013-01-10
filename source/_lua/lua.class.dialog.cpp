#include "_lua/lua.class.dialog.h"
#include "_lua/lua.class.bitmap.h"
#include "_type/type.system.h"


/*##################################
##           Lua-Dialog           ##
##################################*/

//! Constructor
_lua_dialog::_lua_dialog( lua_State* L ) : _dialog()
{ }

//! yesNoDialog
int _lua_dialog::yesNoDialog(lua_State* L)
{
	// static void yesNoDialog( _dialog& d , string msg , string windowLbl , string yesLbl = "Yes" , string noLbl = "No" );
	_dialog::yesNoDialog( *this , luaL_checkstring( L , 1 ) , luaL_checkstring( L , 2 ) , luaL_optstring( L , 3 , _system::getLocalizedString( "lbl_ok" ).c_str() ) , luaL_optstring( L , 4 , _system::getLocalizedString( "lbl_cancel" ).c_str() ) );
	return 0;
}

//! enterTextDialog
int _lua_dialog::enterTextDialog(lua_State* L)
{
	// static void enterTextDialog( _dialog& d , string msg , string windowLbl , string okLbl = "OK" , string cancelLbl = "Cancel" );
	_dialog::enterTextDialog( *this , luaL_checkstring( L , 1 ) , luaL_checkstring( L , 2 ) , luaL_optstring( L , 3 , _system::getLocalizedString( "lbl_ok" ).c_str() ) , luaL_optstring( L , 4 , _system::getLocalizedString( "lbl_cancel" ).c_str() ) );
	return 0;
}

//! imageDialog
int _lua_dialog::imageDialog(lua_State* L)
{
	_lua_bitmap* bmp = Lunar<_lua_bitmap>::check( L , 3 );
	if( bmp && bmp->bm )
		// static void imageDialog( _dialog& d , string msg , string windowLbl , const _bitmap& bmp , string okLbl = "OK" , string otherLbl = "" );
		_dialog::imageDialog( *this , luaL_checkstring( L , 1 ) , luaL_checkstring( L , 2 ) , *(bmp->bm) , luaL_optstring( L , 4 , _system::getLocalizedString( "lbl_ok" ).c_str() ) , luaL_optstring( L , 5 , "" ) );
	else
		_dialog::imageDialog( *this , luaL_checkstring( L , 1 ) , luaL_checkstring( L , 2 ) , _bitmap() , luaL_optstring( L , 4 , _system::getLocalizedString( "lbl_ok" ).c_str() ) , luaL_optstring( L , 5 , "" ) );
	return 0;
}

//! errorDialog
int _lua_dialog::errorDialog(lua_State* L)
{
	// static void errorDialog( _dialog& d , string msg , string windowLbl , string okLbl = "Dismiss" , string otherLbl = "" )
	_dialog::errorDialog( *this , luaL_checkstring( L , 1 ) , luaL_checkstring( L , 2 ) , luaL_optstring( L , 3 , _system::getLocalizedString( "lbl_ok" ).c_str() ) , luaL_optstring( L , 4 , "" ) );
	return 0;
}

//! infoDialog
int _lua_dialog::infoDialog(lua_State* L)
{
	// static void errorDialog( _dialog& d , string msg , string windowLbl , string okLbl = "Dismiss" , string otherLbl = "" )
	_dialog::infoDialog( *this , luaL_checkstring( L , 1 ) , luaL_checkstring( L , 2 ) , luaL_optstring( L , 3 , _system::getLocalizedString( "lbl_ok" ).c_str() ) , luaL_optstring( L , 4 , "" ) );
	return 0;
}

//! warningDialog
int _lua_dialog::warningDialog(lua_State* L)
{
	// static void errorDialog( _dialog& d , string msg , string windowLbl , string okLbl = "Dismiss" , string otherLbl = "" )
	_dialog::infoDialog( *this , luaL_checkstring( L , 1 ) , luaL_checkstring( L , 2 ) , luaL_optstring( L , 3 , _system::getLocalizedString( "lbl_dismiss" ).c_str() ) , luaL_optstring( L , 4 , "" ) );
	return 0;
}

//! execute
int _lua_dialog::execute(lua_State* L){ _dialog::execute(); return 0; }

//! terminate
int _lua_dialog::terminate(lua_State* L){ _dialog::terminate(); return 0; }

//! getIntResult
int _lua_dialog::getIntResult(lua_State* L){ lua_pushnumber( L , _dialog::getIntResult() ); return 1; }

//! getStrResult
int _lua_dialog::getStrResult(lua_State* L){ lua_pushstring( L , _dialog::getStrResult().c_str() ); return 1; }

//! onExit
int _lua_dialog::onExit(lua_State* L){ _dialog::onExit( new _luaCallback( L , 1 ) ); return 0; }

//! hasFinished
int _lua_dialog::hasFinished(lua_State* L){ lua_pushboolean( L , _dialog::hasFinished() ); return 1; }

//! wasAborted
int _lua_dialog::wasAborted(lua_State* L){ lua_pushboolean( L , _dialog::wasAborted() ); return 1; }

//! Lua-_rect
const char _lua_dialog::className[] = "_dialog";
Lunar<_lua_dialog>::FunctionType _lua_dialog::methods[] = {
	LUA_CLASS_FUNC( _lua_dialog , yesNoDialog ),
	LUA_CLASS_FUNC( _lua_dialog , enterTextDialog ),
	LUA_CLASS_FUNC( _lua_dialog , imageDialog ),
	LUA_CLASS_FUNC( _lua_dialog , errorDialog ),
	LUA_CLASS_FUNC( _lua_dialog , infoDialog ),
	LUA_CLASS_FUNC( _lua_dialog , warningDialog ),
	LUA_CLASS_FUNC( _lua_dialog , execute ),
	LUA_CLASS_FUNC( _lua_dialog , terminate ),
	LUA_CLASS_FUNC( _lua_dialog , execute ),
	LUA_CLASS_FUNC( _lua_dialog , onExit ),
	LUA_CLASS_FUNC_END
};

Lunar<_lua_dialog>::PropertyType _lua_dialog::properties[] = {
	{ "intResult" , &_lua_dialog::getIntResult , nullptr },
	{ "strResult" , &_lua_dialog::getStrResult , nullptr },
	{ "aborted" , &_lua_dialog::wasAborted , nullptr },
	{ "finished" , &_lua_dialog::hasFinished , nullptr },
	LUA_CLASS_ATTR_END
};