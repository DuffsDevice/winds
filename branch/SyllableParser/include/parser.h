#ifndef _PARSER_H_
#define _PARSER_H_

#include <string.h>
#include <malloc.h>
#include <list>

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
static const char* combinVocalBefore = "chts\0fts\0lbst\0bs\0rb\0lf\0mpf\0rkt\0ngs\0nd\0ck\0ng\0ch\0sch\0ft\0rz\0";

// means, that the break is before the combination
// Any vocal before is considered not belonging to the syllable
static const char* combinStart = "ch\0sp\0zw\0gl\0cl\0kr\0br\0sch\0qu\0tr\0pf\0fl\0bs\0bl\0ps\0fr\0wr\0kn\0gr\0kl\0kr\0pr\0";

class syllableParser{
	
	private:
		
		// Private Constructor
		syllableParser();
		
		// Functions to check is a character belongs to a certain group
		// Returns the number of chars ahead matching the mask
		static unsigned char isConsonant( char c ){
			return charisof( c , consonants );
		}
		static unsigned char isVocale( char c ){
			return charisof( c , vocales );
		}
		static unsigned char isDiphtong( const char* txt )
		{
			return checkMatchList( txt , diphthongs );
		}
		static unsigned char isCombinStart( const char* txt )
		{
			return checkMatchList( txt , combinStart );
		}
		static unsigned char isCombinVocalBefore( const char* txt )
		{
			return checkMatchList( txt , combinVocalBefore );
		}
		
		// Parse all words
		static std::list<int> parseTextInternal( const char* text , const char* end );
		
		
		// Parse a specific word
		static std::list<int> parseWordInternal( const char* startPos , const char* endPos );
	
	public:
		
		// Returns a pointer to the first occourence of any of the by 'match' specified characters
		static const char* findFirstOf( const char* text , const char* match );
		
		// Check the 'txt'-string for any phrases in the list 'curExpr'
		static unsigned char checkMatchList( const char* txt , const char* curExpr )
		{
			do
			{
				unsigned char i = strlen( curExpr );
				if( strncmp( curExpr , txt , i ) == 0 )
					return i;
				curExpr += i+1;
			}while( *curExpr );
			return 0;
		}
		
		static std::list<int> parseText( const char* txt , const char* end = nullptr )
		{
			int i = std::max( (size_t)1 , strlen( txt ) );
			
			char* text = (char*) malloc( i );
			char* tmp = text;
			
			if( !end )
				end = text + i;
			else
				end = text + ( end - txt );
			
			// Make lowercase
			do
				*tmp++ = tolower( *txt++ );
			while( --i );
			
			std::list<int> lst = parseTextInternal( text , end );
			
			free( text );
			
			return lst;
		}
};

#endif