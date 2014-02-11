// Check if already included
#ifndef _WIN_L_REGISTRY_
#define _WIN_L_REGISTRY_

#include "_lua/lua.lunar.h"
#include "_lua/lua.class.ini.file.h"
#include "_type/type.registry.h"

/**
 * Proxy Classes
 */
class _lua_registry : public _lua_inifile
{
	public:
		
		//! Get underlying _gadget instance
		_registry* getRegistry(){ return (_registry*)this->ini; }
		operator _registry&(){ return *(_registry*)this->ini; }
		
		//! Constructor
		_lua_registry( lua_State* L );
		
		//! C-Ctor
		_lua_registry( _registry* );
		_lua_registry( const _registry* );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_registry>::FunctionType methods[];
		static Lunar<_lua_registry>::PropertyType properties[];
};

#endif