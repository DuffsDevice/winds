#ifndef _WIN_T_REGISTRY_
#define _WIN_T_REGISTRY_

#include "_type/type.h"
#include "_type/type.ini.h"
#include "_type/type.direntry.h"


class _registry : protected _direntry
{
	protected:
		
		_ini* ini; // Ini- Decoder and Encoder
		bool creation; // Flag whether the registry is created this time
		
	public:
		
		//! Ctor
		_registry( _direntry&& file );
		_registry( string content );
		
		//! Flush the content to disk
		void flush();
		
		//! Virtual dtor
		virtual ~_registry();
		
		//! Read an index from the registry out of the supplied section
		string readIndex( string section , string name ) const ;
		_int readIndexInt( string section , string name ) const ;
		
		//! Write an index to the registry, a section and a key will be generated, if they're not existent
		void writeIndex( string section , string name , string value ){ this->ini->getMutableMap()[section][name] = value; }
		
		//! Delete a section from the ini structure
		void deleteSection( string section ){ this->ini->getMutableMap().erase( section ); }
		
		//! Delete an index out of the supplied region
		void deleteIndex( string section , string name );
		
		//! Receive an _iniStructure-object
		const _iniStructure& getMap(){ return this->ini->getMap(); }
		
		//! Receive a modifyable version of the _iniStructure-object
		_iniStructure& getMutableMap(){ return this->ini->getMutableMap(); }
};

#endif
	