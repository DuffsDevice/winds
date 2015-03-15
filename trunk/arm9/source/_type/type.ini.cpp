#include <_type/type.ini.h>
#include <_type/type.tokenizer.h>
#include <_type/type.color.h>

#undef unused
#undef deprecated
#include <nds.h>

const wstring& _ini::readIndex( const wstring& section , const wstring& name , bool* exists ) const
{
	auto it1 = this->array.find( section );	
	
	// Check if section available
	if( it1 != this->array.end() )
	{
		auto it2 = it1->second.find( name );
		
		if( it2 != it1->second.end() )
		{
			if( exists )
				*exists = true;
			return it2->second;
		}
	}
	
	if( exists )
		*exists = false;
	
	static const wstring output;
	return output;
}

void _ini::deleteIndex( const wstring& section , const wstring& name )
{
	auto it1 = this->array.find( section );	
	
	// Check if section available
	if( it1 != this->array.end() )
		it1->second.erase(name);
}

const _assocVector<wstring,wstring>& _ini::readSection( const wstring& section , bool* exists ) const
{
	auto it = this->array.find( section );	
	
	// Check if section available
	if( it != this->array.end() )
	{
		if( exists )
			*exists = true;
		return it->second;
	}
	
	if( exists )
		*exists = false;
	static const _assocVector<wstring,wstring> output;
	return output;
}

_s16 _ini::read( const wstring& input )
{
	this->array.clear();
	
	wstring							line;
	_assocVector<wstring,wstring>*	section = &this->array[L"_global_"];
	_u16 							lineNo = 0;
	_u16 							errorNo = -1; // Line with error
	_wtokenizer						tok = _wtokenizer( input , line , L"\n\r" , true );
	
	/* Scan through file line by line */
	while( tok.next() )
	{
		lineNo++;
		
		/**
		 * Choose appropriate Action
		 */
        if( line.front() == L';' || line.front() == L'#' )
		{
            /* Per Python ConfigParser, allow '#' and ';' comments at start of line */
        }
        else if( line.front() == '[' )
		{
            /* A "[section]" line */
			size_t end = line.rawFind(L']');
			
            if ( end != wstring::npos )
				section = &this->array[ line.rawSubstr( line.getIndexBytes(0) , end - line.getIndexPreBytes(end) ) ];
			else
				errorNo = lineNo;
        }
        else
		{
			/* Must be a name:= value pair! */
			size_t delim = line.rawFind_first_of(L":=");
			
			if( delim == wstring::npos ){
				errorNo = lineNo;
				continue;
			}
			
			wstring value = line.rawSubstr( delim + line.getIndexBytes( delim ) , line.rawFind_first_of( L";#" , delim ) );
			line.rawErase( delim , wstring::npos ); // Will leave over the key
			
			trim( line );
			trim( value );
			
			(*section)[line] = move(value);
		}
    }
	
	return errorNo;
}

_int _ini::readIndexInt( const wstring& section , const wstring& name , bool* exists ) const 
{
	const wstring&	attr = this->readIndex( section , name , exists );
	wstring			firstThree = attr.rawSubstr( 0 , 3 );
	
	// Allow formats of RGB( 14 , 6 , 9 ) and rgba( 1 , 20 , 25 , 0 )
	if( firstThree == "RGB" || firstThree == "rgb" )
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
			return -1;
		
		for( ; !isdigit( attr[i] ) && attr.length() > i ; i++ ); // Go to first number
			f = i; // Set Mark
		if( !isdigit( attr[i] ) ) // Check if we reached the end of the wstring
			return 0;
		for( ; isdigit( attr[i] ) ; i++ ); // Go to end of number
		r = string2int( attr.substr( f , i-f ).c_str() ); // Save red part
		
		for( ; !isdigit( attr[i] ) && attr.length() > i ; i++ ); // Go to second number
			f = i; // Set Mark
		if( !isdigit( attr[i] ) ) // Check if we reached the end of the wstring
			return 0;
		for( ; isdigit( attr[i] ) ; i++ ); // Go to end of number
		g = string2int( attr.substr( f , i-f ).c_str() ); // Save green part
		
		for( ; !isdigit( attr[i] ) && attr.length() > i ; i++ ); // Go to third number
			f = i; // Set Mark
		if( !isdigit( attr[i] ) ) // Check if we reached the end of the wstring
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

wstring _ini::write()
{
	wstring output;
	
	for( auto& section : this->array )
	{
		if( section.first != L"_global_" )
		{
			output.push_back( L'[' );
			output += section.first;
			output += L"]\r\n";
		}
		for( auto& nvp : section.second )
		{
			if( !nvp.first.length() )
				continue;
			output += nvp.first;
			output.push_back( L'=' );
			output += nvp.second;
			output += L"\r\n";
		}
		output += L"\r\n";
	}
	
	return move( output );
}