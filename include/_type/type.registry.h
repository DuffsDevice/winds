#ifndef _WIN_T_REGISTRY_
#define _WIN_T_REGISTRY_

#include "_type/type.h"
#include "_type/type.ini.h"
#include "_type/type.direntry.h"

class _registry : private _direntry
{
	private:
		
		_ini* ini;
	
	public:
		
		_registry( string filename ) :
			_direntry( filename )
			, ini( nullptr )
		{
			if( !this->exists )
				this->create();
				
			this->openread();
			this->ini = new _ini( this->readString() );
			this->close();
			
			this->ini->read();
			
		}
		
		~_registry()
		{
			if( this->ini )
			{
				this->ini->write();
				this->openwrite( true );
				this->writeString( this->ini->getString() );
				this->close();
				delete this->ini;
			}
		}
		
		string readIndex( string section , string name )
		{
			return this->ini->getMap()[section][name];
		}
		
		void writeIndex( string section , string name , string value )
		{
			this->ini->getMap()[section][name] = value;
		}
		
		void deleteSection( string section )
		{
			this->ini->getMap().erase( section );
		}
		
		void deleteIndex( string section , string name )
		{
			this->ini->getMap()[section].erase(name);
		}
		
		_iniStructure& getMap(){
			return this->ini->getMap();
		}
};

#endif
	