// Check if already included
#ifndef _WIN_L_IMAGEGADGET_
#define _WIN_L_IMAGEGADGET_

#include <_lua/lua.lunar.h>
#include <_lua/lua.class.gadget.h>
#include <_lua/lua.class.bitmap.h>
#include <_gadget/gadget.image.h>

/**
 * Proxy Classes
 */
class _lua_imagegadget : public _lua_gadget {
	
	public:
		
		_lua_imagegadget( lua_State* L );
		
		_lua_imagegadget( _imageGadget* c ) : _lua_gadget( c , false )
		{ }
		
		//! getImage
		int getImage( lua_State* L );
		
		//! getImage
		int setImage( lua_State* L );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_imagegadget>::FunctionType	methods[];
		static Lunar<_lua_imagegadget>::PropertyType	properties[];
		using baseclasses = Lunar<_lua_imagegadget>::BaseClassType<_lua_gadget>;
};

#endif