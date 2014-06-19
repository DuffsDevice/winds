#include "_lua/lua.controller.font.h"
#include "_lua/lua.func.h"
using namespace _luafunc;

int _lua_fontcontroller::getFont(lua_State* L){
	return push( L , _fontController::getFont( check<_literal>( L , 1 ) ) );
}

int _lua_fontcontroller::getStandardFont(lua_State* L){
	return push( L , _fontController::getStandardFont() );
}

int _lua_fontcontroller::setStandardFont(lua_State* L){
	_fontController::setStandardFont( check<_fontHandle>( L , 1 ) );
	return 0;
}

int _lua_fontcontroller::getStandardFontSize(lua_State* L){
	return push( L , _fontController::getStandardFontSize() );
}

int _lua_fontcontroller::setStandardFontSize(lua_State* L){
	_fontController::setStandardFontSize( check<_u8>( L , 1 ) );
	return 0;
}

int _lua_fontcontroller::deleteFont(lua_State* L){
	_fontController::deleteFont( check<_fontHandle>( L , 1 ) );
	return 0;
}

int _lua_fontcontroller::getRegisteredFonts(lua_State* L){
	return push( L , _fontController::getRegisteredFontsAsHandles() );
}

//! Lua-_fontController
const char _lua_fontcontroller::className[] = "FontController";
Lunar<_lua_fontcontroller>::FunctionType _lua_fontcontroller::methods[] = {
	LUA_CLASS_FUNC_END
};

Lunar<_lua_fontcontroller>::PropertyType _lua_fontcontroller::properties[] = {
	LUA_CLASS_ATTR_END
};

Lunar<_lua_fontcontroller>::StaticType _lua_fontcontroller::staticmethods[] = {
	{ "getFont"				, &_lua_fontcontroller::getFont } ,
	{ "getStandardFont"		, &_lua_fontcontroller::getStandardFont } ,
	{ "setStandardFont"		, &_lua_fontcontroller::setStandardFont } ,
	{ "getStandardFontSize"	, &_lua_fontcontroller::getStandardFontSize } ,
	{ "setStandardFontSize"	, &_lua_fontcontroller::setStandardFontSize } ,
	{ "deleteFont"			, &_lua_fontcontroller::deleteFont } ,
	{ "getRegisteredFonts"	, &_lua_fontcontroller::getRegisteredFonts }
};