#include "_lua/lua.controller.gui.h"
#include "_screen/screen.desktop.h"
#include "_screen/screen.bootup.h"
#include "_screen/screen.startup.h"
#include "_lua/lua.func.h"
using namespace _luafunc;

int _lua_guicontroller::getState(lua_State* L){
	return push( L , guiState2string[_guiController::getState()] );
}

int _lua_guicontroller::changeState(lua_State* L){
	_guiController::changeState( string2guiState[check<string>( L , 1 )] );
	return 0;
}

int _lua_guicontroller::getHost(lua_State* L){
	return push( L , _guiController::getHost() );
}

//int _lua_guicontroller::getTopScreen(lua_State* L){
//	return push( L , _guiController::getHost() );
//}

int _lua_guicontroller::setHost(lua_State* L)
{
	string hostType = check<string>( L , 1 );
	if( hostType == "desktopScreen" )
		_guiController::setHost<_desktopScreen>( lightcheck<_style>( L , 2 ) );
	else if( hostType == "bootupScreen" )
		_guiController::setHost<_bootupScreen>( lightcheck<_style>( L , 2 ) );
	else if( hostType == "startupScreen" )
		_guiController::setHost<_desktopScreen>( lightcheck<_style>( L , 2 ) );
	else
		_guiController::deleteHost();
	
	return 0;
}

int _lua_guicontroller::enableKeyboard(lua_State* L){
	_guiController::enableKeyboard( optcheck<_coord>( L , 1 ) );
	return 0;
}

int _lua_guicontroller::disableKeyboard(lua_State* L){
	_guiController::disableKeyboard();
	return 0;
}

int _lua_guicontroller::getKeyboard(lua_State* L){
	return push( L , _guiController::getKeyboard() );
}

int _lua_guicontroller::getCurrentFocus(lua_State* L){
	return push( L , _guiController::getCurrentFocus() );
}

int _lua_guicontroller::getLastClickedGadget(lua_State* L){
	return push( L , _guiController::getLastClickedGadget() );
}

int _lua_guicontroller::isKeyboardMagnifEnabled(lua_State* L){
	return push( L , _guiController::isKeyboardMagnifEnabled() );
}

// Design
int _lua_guicontroller::getItemFg(lua_State* L){
	return push( L , _guiController::getItemFg( lightcheck<bool>( L , 1 ) , lightcheck<bool>( L , 2 ) ) );
}

int _lua_guicontroller::getItemBg(lua_State* L){
	return push( L , _guiController::getItemBg( lightcheck<bool>( L , 1 ) , lightcheck<bool>( L , 2 ) ) );
}

int _lua_guicontroller::getControlFg(lua_State* L){
	return push( L , _guiController::getControlFg() );
}

int _lua_guicontroller::getControlBg(lua_State* L){
	return push( L , _guiController::getControlBg() );
}

int _lua_guicontroller::getWindowsDesign(lua_State* L){
	return push( L , _guiController::getWindowsDesign( lightcheck<bool>( L , 1 , true ) ) );
}

int _lua_guicontroller::enableKeyboardMagnif(lua_State* L){
	_guiController::enableKeyboardMagnif();
	return 0;
}

int _lua_guicontroller::disableKeyboardMagnif(lua_State* L){
	_guiController::disableKeyboardMagnif();
	return 0;
}

int _lua_guicontroller::getDisabledPalette(lua_State* L){
	//_guiController::getDisabledPalette(); //!@todo
	return 0;
}

//! Lua-_fontController
const char _lua_guicontroller::className[] = "GuiController";
Lunar<_lua_guicontroller>::FunctionType _lua_guicontroller::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_guicontroller>::PropertyType _lua_guicontroller::properties[] = {
	LUA_CLASS_ATTR_END
};

Lunar<_lua_guicontroller>::StaticType _lua_guicontroller::staticmethods[] = {
	{ "getState"				, &_lua_guicontroller::getState } ,
	{ "changeState"				, &_lua_guicontroller::changeState } ,
	{ "getHost"					, &_lua_guicontroller::getHost } ,
	{ "setHost"					, &_lua_guicontroller::setHost } ,
	{ "enableKeyboard"			, &_lua_guicontroller::enableKeyboard } ,
	{ "disableKeyboard"			, &_lua_guicontroller::disableKeyboard } ,
	{ "getKeyboard"				, &_lua_guicontroller::getKeyboard } ,
	{ "getCurrentFocus"			, &_lua_guicontroller::getCurrentFocus } ,
	{ "getLastClickedGadget"	, &_lua_guicontroller::getLastClickedGadget } ,
	{ "isKeyboardMagnifEnabled"	, &_lua_guicontroller::isKeyboardMagnifEnabled } ,
	{ "enableKeyboardMagnif"	, &_lua_guicontroller::enableKeyboardMagnif } ,
	{ "disableKeyboardMagnif"	, &_lua_guicontroller::disableKeyboardMagnif } ,
	{ "getItemFg"				, &_lua_guicontroller::getItemFg } ,
	{ "getItemBg"				, &_lua_guicontroller::getItemBg } ,
	{ "getControlFg"			, &_lua_guicontroller::getControlFg } ,
	{ "getControlBg"			, &_lua_guicontroller::getControlBg } ,
	{ "getWindowsDesign"		, &_lua_guicontroller::getWindowsDesign }
};