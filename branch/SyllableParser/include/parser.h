#ifndef _PARSER_H_
#define _PARSER_H_

#include <string.h>
#include <malloc.h>
#include <list>
#include <stdio.h>

static bool charisof( char c , const char* txt )
{
	do
	{
		if( *txt == c )
			return true;		
	}while( *++txt );
	
	return false;
}

static inline char tolower( char c )
{
	if( c >= 'A' && c <= 'Z' )
		return c|32;
	return c;
}

static const char vocales[] = "aeiouyöäü";
static const char consonants[] = "bcdfghjklmnpqrstvwxzß";

// Vocals that are speaken as one syllable
static const char* diphthongs = "eu\0ei\0ie\0au\0äu\0";

// means, that a vocal has to go first, and the syllable break is behind the combination
static const char* combinVocalBefore = "chts\0fts\0lbst\0bs\0rb\0lf\0mpf\0rkt\0ngs\0ck\0ng\0ch\0sch\0ft\0rz\0";

// means, that the break is before the combination
// Any vocal before is considered not belonging to the syllable
static const char* combinStart = "ch\0sp\0zw\0gl\0cl\0kr\0br\0sch\0qu\0tr\0pf\0fl\0bs\0bl\0ps\0fr\0wr\0kn\0gr\0kl\0kr\0pr\0";

class parser{
	
	private:
		
		// Private Constructor
		parser();
		
		// Functions to check is a character belongs to a certain group
		// Returns the number of chars ahead matching the mask
		static unsigned char isConsonant( char c ){
			return charisof( tolower( c ) , consonants );
		}
		static unsigned char isVocale( char c ){
			return charisof( tolower( c ) , vocales );
		}
		static unsigned char isDiphtong( const char* txt );
		static unsigned char isCombinStart( const char* txt );
		static unsigned char isCombinVocalBefore( const char* txt );
		
		// Parse all words
		static std::list<int> parseTextInternal( const char* text );
		
		
		// Parse a specific word
		static std::list<int> parseWordInternal( const char* startPos , const char* endPos );
	
	public:
		
		static std::list<int> parseText( const char* txt )
		{
			int i = std::max( (size_t)1 , strlen( txt ) );
			
			char* text = (char*) malloc( i );
			char* tmp = text;
			
			// Make lowercase
			do
				*tmp++ = tolower( *txt++ );
			while( --i );
			
			std::list<int> lst = parseTextInternal( text );
			
			free( text );
			
			return lst;
		}
};

#endif