#include "_type/type.registry.h"

_registry::_registry( _direntry&& file ) :
	_direntry( file )
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

_registry::_registry( string content ) :
	_direntry( "" )
	, ini( new _ini( content ) )
	, creation( false )
{
	this->ini->read();
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

string _registry::readIndex( string section , string name ) const
{
	const _iniStructure& m = this->ini->getMap();
	
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

_int _registry::readIndexInt( string section , string name ) const 
{
	string attr = this->readIndex( section , name );
	
	// Allow formats of RGB( 14 , 6 , 9 ) and rgba( 1 , 20 , 25 , 0 )
	if( attr.substr( 0 , 3 ) == "RGB" || attr.substr( 0 , 3 ) == "rgb" )
	{
		_u8 i = 2 , f = 0;
		bool hasAlpha = false;
		_u8 r , b , g , a = 1;
		
		if( isalpha( attr[3] ) && ( attr[3] == 'A' || attr[3] == 'a' ) )
		{
			i = 3;
			hasAlpha = true;
		}
		else if( isalpha( attr[3] ) ) // No RGB
			return string2int( attr.c_str() );
		
		for( ; !isdigit( attr[i] ) && attr.length() > i ; i++ ); // Go to first number
		f = i; // Set Mark
		if( !isdigit( attr[i] ) ) // Check if we reached the end of the string
			return 0;
		for( ; isdigit( attr[i] ) ; i++ ); // Go to end of number
		r = string2int( attr.substr( f , i-f ).c_str() ); // Save red part
		
		for( ; !isdigit( attr[i] ) && attr.length() > i ; i++ ); // Go to second number
		f = i; // Set Mark
		if( !isdigit( attr[i] ) ) // Check if we reached the end of the string
			return 0;
		for( ; isdigit( attr[i] ) ; i++ ); // Go to end of number
		g = string2int( attr.substr( f , i-f ).c_str() ); // Save green part
		
		for( ; !isdigit( attr[i] ) && attr.length() > i ; i++ ); // Go to third number
		f = i; // Set Mark
		if( !isdigit( attr[i] ) ) // Check if we reached the end of the string
			return 0;
		for( ; isdigit( attr[i] ) ; i++ ); // Go to end of number
		b = string2int( attr.substr( f , i-f ).c_str() ); // Save blue part
		
		if( hasAlpha )
		{
			for( ; !isdigit( attr[i] ) && attr.length() > i ; i++ ); // Go to fourth number
			if( attr[i] == '0' )
				a = 0;
		}
		
		return RGBA( r , g , b , a );
	}
	else if( attr == "true" )
		return 1;
	else if( attr == "false" )
		return 0;
	
	return string2int( attr.c_str() );
}

void _registry::deleteIndex( string section , string name )
{
	_iniStructure& m = this->ini->getMutableMap();
	
	auto it1 = m.find( section );	
	
	// Check if section available
	if( it1 != m.end() )
		it1->second.erase(name);
}