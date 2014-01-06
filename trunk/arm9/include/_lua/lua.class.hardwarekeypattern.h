// Check if already included
#ifndef _WIN_L_HARDWAREKEYS_
#define _WIN_L_HARDWAREKEYS_

#include "_lua/lua.lunar.h"
#include "_type/type.h"
#include "_type/type.key.h"

/**
 * Proxy Classes
 */
class _lua_hardwarekeypattern : public _hardwareKeyPattern
{
	public:
		
		_lua_hardwarekeypattern( const _hardwareKeyPattern& );
		
		//! Constructor
		_lua_hardwarekeypattern( lua_State* L );
		
		//! isHardwareKey
		static int isHardwareKey(lua_State* L);
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_hardwarekeypattern>::FunctionType	methods[];
		static Lunar<_lua_hardwarekeypattern>::PropertyType	properties[];
		static Lunar<_lua_hardwarekeypattern>::StaticType	staticmethods[];
	
};

#endif