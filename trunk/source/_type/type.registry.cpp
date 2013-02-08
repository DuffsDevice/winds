#include "_type/type.registry.h"

_registry::_registry( string filename ) :
	_direntry( filename )
	, ini( nullptr )
	, creation( false )
{
	// Obtain size of the .reg file
	_u32 size = this->getSize();
	
	if( !this->isExisting() || !size )
	{
		this->create();
		this->ini = new _ini("");
		this->writeIndex( "_global_" , ".firstTimeUse" , "1" );
		this->creation = true;
	}
	else
	{
		this->ini = new _ini( this->readString( size ) );
		this->ini->read();
		this->deleteIndex( "_global_" , ".firstTimeUse" );
	}	
}

void _registry::flush()
{
	if( this->ini )
	{
		this->ini->write();
		this->openwrite( true );
		this->writeString( this->ini->getString() );
		this->close();
	}
}

_registry::~_registry()
{
	this->flush();
	
	if( this->ini )
		delete this->ini;
}

string _registry::readIndex( string section , string name )
{
	_iniStructure& m = this->ini->getMap();
	
	auto it1 = m.find( section );	
	
	// Check if section available
	if( it1 != m.end() )
	{
		auto it2 = it1->second.find( name );
		
		if( it2 != it1->second.end() )
			return it2->second;
	}
	return "";
}

void _registry::deleteIndex( string section , string name )
{
	_iniStructure& m = this->ini->getMap();
	
	auto it1 = m.find( section );	
	
	// Check if section available
	if( it1 != m.end() )
		it1->second.erase(name);
}