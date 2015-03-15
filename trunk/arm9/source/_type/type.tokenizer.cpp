#include <_type/type.tokenizer.h>
#include <cstdio>

bool _tokenizer::next()
{
	do
	{
		start = end;
		
		if( start == string::npos )
			return false;
		
		// Find Delimiter
		end = source.find_first_of( delimiters , start );
		
		if( end != string::npos ){ // Check if new delimiter found
			destination = source.substr( start , end - start ); // Get text from Start (start) to End (end)
			end++; // Move beyond delimiter
		}
		else
			destination = source.substr( start );
		
	}while( noEmptyValues && destination.empty() );
	
	return true;
}

bool _wtokenizer::next()
{
	do
	{
		start = end;
		
		if( start == wstring::npos )
			return false;
		
		// Find Delimiter
		end = source.rawFind_first_of( delimiters , start );
		
		if( end != wstring::npos ){ // Check if new delimiter found
			destination = source.rawSubstr( start , end - start ); // Get text from Start (start) to End (end)
			end += source.getIndexBytes( end ); // Move beyond delimiter
		}
		else
			destination = source.rawSubstr( start , wstring::npos );
		
	}while( noEmptyValues && destination.empty() );
	
	return true;
}

_vector<string> tokenize( const string& input , _literal separators , bool noEmptyElements )
{
	_vector<string> result;
	
	string line;
	_tokenizer tok = _tokenizer( input , line , separators , noEmptyElements );
	
	while( tok.next() )
		result.emplace_back( move(line) );
	
	return result;
}
_vector<wstring> tokenize( const wstring& input , _wliteral separators , bool noEmptyElements )
{
	_vector<wstring> result;
	
	wstring line;
	_wtokenizer tok = _wtokenizer( input , line , separators , noEmptyElements );
	
	while( tok.next() )
		result.emplace_back( move(line) );
	
	return result;
}