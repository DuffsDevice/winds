#include <_type/type.ini.h>
#include <_type/type.tokenizer.h>
#include <_type/type.color.h>

const string& _ini::readIndex( const string& section , const string& name ) const
{
	auto it1 = this->array.find( section );	
	
	// Check if section available
	if( it1 != this->array.end() )
	{
		auto it2 = it1->second.find( name );
		
		if( it2 != it1->second.end() )
			return it2->second;
	}
	static const string output;
	return output;
}

void _ini::deleteIndex( const string& section , const string& name )
{
	auto it1 = this->array.find( section );	
	
	// Check if section available
	if( it1 != this->array.end() )
		it1->second.erase(name);
}

const _assocVector<string,string>& _ini::readSection( const string& section ) const
{
	auto it = this->array.find( section );	
	
	// Check if section available
	if( it != this->array.end() )
		return it->second;
	
	static const _assocVector<string,string> output;
	return output;
}

_s16 _ini::read( const string& input )
{
	this->array.clear();
	
	string		line, section = "_global_";
	_u16 		lineNo = 0;
	_u16 		errorNo = -1; // Line with error
	_tokenizer	tok = _tokenizer( input , line , "\n\r" , true );
	
	/* Scan through file line by line */
	while( tok.next() )
	{
		/**
		 * Choose appropriate Action
		 */
        if( line[0] == ';' || line[0] == '#' )
		{
            /* Per Python ConfigParser, allow '#' and ';' comments at start of line */
        }
        else if( line[0] == '[' )
		{
            /* A "[section]" line */
			size_t end = line.find_first_of("]");
			
            if ( end != string::npos )
				section = line.substr( 1 , end - 1 );
			else{
				errorNo = lineNo;
				continue;
			}
			
			this->array[section];
        }
        else
		{
			/* Must be a name:= value pair! */
			size_t delim = line.find_first_of(":=");
			
			if( delim == string::npos ){
				errorNo = lineNo;
				continue;
			}
			
			string name = line.substr( 0 , delim );
			string value = line.substr( delim + 1 , line.find_first_of(";#") );
			
			trim( name );
			trim( value );
			
			this->array[section][name] = value;
        }
    }
	
	return errorNo;
}

_int _ini::readIndexInt( const string& section , const string& name ) const 
{
	const string& attr = this->readIndex( section , name );
	
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
		
		return _color::fromRGB( r , g , b , a );
	}
	else if( attr == "true" )
		return 1;
	else if( attr == "false" )
		return 0;
	
	return string2int( attr.c_str() );
}

string _ini::write()
{	
	string output;
	
	for( auto& section : this->array )
	{
		if( section.first != "_global_" )
		{
			output += "[";
			output += section.first;
			output += "]\r\n";
		}
		for( auto& nvp : section.second )
		{
			if( !nvp.first.length() )
				continue;
			output += nvp.first;
			output += " = ";
			output += nvp.second;
			output += "\r\n";
		}
		output += "\r\n";
	}
	
	return move( output );
}