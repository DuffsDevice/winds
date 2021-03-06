#ifndef _WIN_T_TOKENIZER_
#define _WIN_T_TOKENIZER_

#include <_type/type.h>
#include <cstdio>

//! Class for tokenizing a string iteratively
class _tokenizer
{
	private:
		
		const string&	source;
		string&			destination;
		_literal		delimiters;
		_u32			start;
		_u32			end;
		bool			noEmptyValues;
	
	public:
		
		_tokenizer( const string& src , string& dest , _literal delim , bool noEmptyValues = false ) :
			source( src )
			, destination( dest )
			, delimiters( delim )
			, end( 0 )
			, noEmptyValues( noEmptyValues )
		{}
		
		//! Get next token, returns false if there is no token left
		bool next();
};


class _wtokenizer
{
	private:
		
		const wstring&	source;
		wstring&		destination;
		_wliteral		delimiters;
		_u32			start;
		_u32			end;
		bool			noEmptyValues;
	
	public:
		
		_wtokenizer( const wstring& src , wstring& dest , _wliteral delim , bool noEmptyValues = false ) :
			source( src )
			, destination( dest )
			, delimiters( delim )
			, end( 0 )
			, noEmptyValues( noEmptyValues )
		{}
		
		//! Get next token, returns false if there is no token left
		bool next();
};

//! Shortcut method for simple but maybe inefficient string tokenizing
extern _vector<string> tokenize( const string& input , _literal separators , bool noEmptyElements = false );
extern _vector<wstring> tokenize( const wstring& input , _wliteral separators , bool noEmptyElements = false );


//! Join a list of elements in a string, each element separated with 'sep'
template<template<class...> class Container>
string unTokenize( const Container<string>& container , const string& sep = "," , bool sepAtStart = false , bool sepAtEnd = false )
{
	string result;
	bool addSep = sepAtStart;
	for( const string& elem : container )
	{
		if( addSep )
			result += sep;
		else
			addSep = true;
		result += elem;
	}
	if( sepAtEnd && container.size() )
		result += sep;
	return result;
}
template<template<class...> class Container>
wstring unTokenize( const Container<wstring>& container , const wstring& sep = "," , bool sepAtStart = false , bool sepAtEnd = false )
{
	wstring result;
	bool addSep = sepAtStart;
	for( const wstring& elem : container )
	{
		if( addSep )
			result += sep;
		else
			addSep = true;
		result += elem;
	}
	if( sepAtEnd && container.size() )
		result += sep;
	return result;
}

#endif