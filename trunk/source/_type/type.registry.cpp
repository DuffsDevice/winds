#include "_type/type.registry.h"
#include "nds.h"

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
		this->creation = true;
	}
	else
	{
		this->ini = new _ini( this->readString( size ) );
		this->ini->read();
	}	
}

_registry::~_registry()
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

string _registry::readIndex( string section , string name )
{
	_iniStructure& m = this->ini->getMap();
	if( !m.count( section ) || !m[section].count(name) )
		return "";
	return m[section][name];
}

void _registry::writeIndex( string section , string name , string value )
{
	this->ini->getMap()[section][name] = value;
}

void _registry::deleteSection( string section )
{
	this->ini->getMap().erase( section );
}

void _registry::deleteIndex( string section , string name )
{
	this->ini->getMap()[section].erase(name);
}

_iniStructure& _registry::getMap(){
	return this->ini->getMap();
}