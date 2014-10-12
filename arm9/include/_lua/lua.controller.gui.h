// Check if already included
#ifndef _WIN_L_CONTROLLER_GUI_
#define _WIN_L_CONTROLLER_GUI_

#include <_lua/lua.lunar.h>
#include <_controller/controller.gui.h>

/**
 * Proxy Classes
 */
class _lua_guicontroller
{
	public:
		
		//! getState
		static int getState(lua_State* L);
		
		//! changeState
		static int changeState(lua_State* L);
		
		//! getHost
		static int getHost(lua_State* L);
		
		//! setHost
		static int setHost(lua_State* L);
		
		//! enableKeyboard
		static int enableKeyboard(lua_State* L);
		
		//! disableKeyboard
		static int disableKeyboard(lua_State* L);
		
		//! getKeyboard
		static int getKeyboard(lua_State* L);
		
		//! getCurrentFocus
		static int getCurrentFocus(lua_State* L);
		
		//! getLastClickedGadget
		static int getLastClickedGadget(lua_State* L);
		
		//! isKeyboardMagnifEnabled
		static int isKeyboardMagnifEnabled(lua_State* L);
		
		//! enableKeyboardMagnif
		static int enableKeyboardMagnif(lua_State* L);
		
		//! disableKeyboardMagnif
		static int disableKeyboardMagnif(lua_State* L);
		
		//! getItemFg
		static int getItemFg(lua_State* L);
		
		//! gtItemBg
		static int getItemBg(lua_State* L);
		
		//! getControlFg
		static int getControlFg(lua_State* L);
		
		//! getControlBg
		static int getControlBg(lua_State* L);
		
		//! getWindowsDesign
		static int getWindowsDesign(lua_State* L);
		
		//! getDisabledPalette
		static int getDisabledPalette(lua_State* L);
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_guicontroller>::StaticType	staticmethods[];
		static Lunar<_lua_guicontroller>::FunctionType	methods[];
		static Lunar<_lua_guicontroller>::PropertyType	properties[];	
};

#endif