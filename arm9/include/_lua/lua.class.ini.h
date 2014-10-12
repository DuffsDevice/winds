// Check if already included
#ifndef _WIN_L_INI_
#define _WIN_L_INI_

#include <_lua/lua.lunar.h>
#include <_type/type.ini.h>

/**
 * Proxy Classes
 */
class _lua_ini
{
	protected:
		
		_ini*	ini;
		bool	wasAllocated;
	
	public:
		
		//! Get underlying _gadget instance
		_ini* getIni(){ return this->ini; }
		operator _ini&(){ return *this->ini; }
		
		//! Constructor
		_lua_ini( lua_State* L );
		
		//! C-Ctor
		_lua_ini( _ini* , bool wasAllocated = false );
		_lua_ini( const _ini* );
		
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_ini>::FunctionType methods[];
		static Lunar<_lua_ini>::PropertyType properties[];
};

#endif