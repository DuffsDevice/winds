// Check if already included
#ifndef _WIN_L_IMAGEBUTTON_
#define _WIN_L_IMAGEBUTTON_

#include <_lua/lua.lunar.h>
#include <_lua/lua.class.gadget.h>
#include <_lua/lua.gadget.button.h>
#include <_gadget/gadget.button.image.h>

/**
 * Proxy Classes
 */
class _lua_imagebutton : public _lua_gadget
{
	public:
		
		//! Lua-Ctor
		_lua_imagebutton( lua_State* L );
		
		//! C-Ctor
		_lua_imagebutton( _imageButton* b ) : _lua_gadget( b , false )
		{ }
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_imagebutton>::FunctionType	methods[];
		static Lunar<_lua_imagebutton>::PropertyType	properties[];
		using baseclasses = Lunar<_lua_imagebutton>::BaseClassType<_lua_button>;
};

#endif