#include "_lua/lua.controller.font.h"
#include "_lua/lua.func.h"
using namespace _luafunc;

int _lua_fontcontroller::getFont(lua_State* L){
	
}

int _lua_fontcontroller::getStandardFont(lua_State* L){
	
}

int _lua_fontcontroller::setStandardFont(lua_State* L){
	
}

int _lua_fontcontroller::getStandardFontSize(lua_State* L){
	
}

int _lua_fontcontroller::setStandardFontSize(lua_State* L){
	
}

int _lua_fontcontroller::deleteFont(lua_State* L){
	
}

int _lua_fontcontroller::getRegisteredFonts(lua_State* L){
	
}

//! Lua-_fontController
const char _lua_fontcontroller::className[] = "FontController";

Lunar<_lua_fontcontroller>::StaticType _lua_fontcontroller::staticmethods[] = {
	{ "getFont"				, &_lua_fontcontroller::getFont } ,
	{ "getStandardFont"		, &_lua_fontcontroller::getStandardFont } ,
	{ "setStandardFont"		, &_lua_fontcontroller::setStandardFont } ,
	{ "getStandardFontSize"	, &_lua_fontcontroller::getStandardFontSize } ,
	{ "setStandardFontSize"	, &_lua_fontcontroller::setStandardFontSize } ,
	{ "deleteFont"			, &_lua_fontcontroller::deleteFont } ,
	{ "getRegisteredFonts"	, &_lua_fontcontroller::getRegisteredFonts }
};