#ifndef _WIN_T_REGISTRY_
#define _WIN_T_REGISTRY_

#include "_type/type.h"
#include "_type/type.ini.h"
#include "_type/type.direntry.h"

class _registry : protected _direntry
{
	protected:
		
		_ini* ini;
		bool creation;
		
	public:
		
		_registry( string filename );
		
		~_registry();
		
		string readIndex( string section , string name );
		
		void writeIndex( string section , string name , string value );
		
		void deleteSection( string section );
		
		void deleteIndex( string section , string name );
		
		_iniStructure& getMap();
};

#endif
	