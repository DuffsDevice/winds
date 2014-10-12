// Check if already included
#ifndef _WIN_L_CONTROLLER_FONT_
#define _WIN_L_CONTROLLER_FONT_

#include <_lua/lua.lunar.h>
#include <_controller/controller.font.h>

/**
 * Proxy Classes
 */
class _lua_fontcontroller
{
	public:
		
		//! getFont
		static int getFont(lua_State* L);
		
		//! getStandardFont
		static int getStandardFont(lua_State* L);
		
		//! setStandardFont
		static int setStandardFont(lua_State* L);
		
		//! getStandardFontSize
		static int getStandardFontSize(lua_State* L);
		
		//! setStandardFontSize
		static int setStandardFontSize(lua_State* L);
		
		//! deleteFont
		static int deleteFont(lua_State* L);
		
		//! getRegisteredFonts
		static int getRegisteredFonts(lua_State* L);
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_fontcontroller>::StaticType	staticmethods[];
		static Lunar<_lua_fontcontroller>::FunctionType	methods[];
		static Lunar<_lua_fontcontroller>::PropertyType	properties[];
	
};

#endif