// Check if already included
#ifndef _WIN_L_INIFILE_
#define _WIN_L_INIFILE_

#include <_lua/lua.lunar.h>
#include <_lua/lua.class.ini.h>
#include <_lua/lua.class.direntry.h>
#include <_type/type.ini.file.h>

/**
 * Proxy Classes
 */
class _lua_inifile : public _lua_ini
{
	public:
		
		//! Get underlying _ini instance
		_iniFile* getIni(){ return (_iniFile*)this->ini; }
		operator _iniFile&(){ return *(_iniFile*)this->ini; }
		operator _direntry&(){ return *(_direntry*)(_iniFile*)this->ini; }
		
		//! Constructor
		_lua_inifile( lua_State* L );
		
		//! C-Ctor
		_lua_inifile( _iniFile* , bool wasAllocated = false );
		_lua_inifile( const _iniFile* );
		
		//! Lunar !//
		static const char className[];
		static Lunar<_lua_inifile>::FunctionType methods[];
		static Lunar<_lua_inifile>::PropertyType properties[];
		using baseclasses = Lunar<_lua_inifile>::BaseClassType<_lua_ini,_lua_direntry>;
};

#endif