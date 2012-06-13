#include "_type/type.registry.h"

_registry::_registry( string filename ) :
	_direntry( filename )
	, ini( nullptr )
	, creation( false )
{
	if( !this->exists )
	{
		this->create();
		this->ini = new _ini("");
		this->creation = true;
	}
	else
	{
		this->openread();
		this->ini = new _ini( this->readString() );
		this->close();
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
	return this->ini->getMap()[section][name];
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