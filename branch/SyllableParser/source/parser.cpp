#include "parser.h"
#include <stdio.h>

const char* syllableParser::findFirstOf( const char* text , const char* match )
{
	if( !*text )
		return nullptr;
	
	do
	{
		const char* tmp = match;
		do
		{
			if( *text == *tmp )
				return text;
		}while( *++tmp );
	}while( *++text );
	
	return nullptr;
}

std::list<int> syllableParser::parseTextInternal( const char* txt , const char* end )
{
	const char* startPos;
	const char* endPos;
	// Current poition of iteration
	const char* curPos = txt;
	
	std::list<int> ret;
	
	do
	{
		const char* nextEnd = findFirstOf( curPos , " ,\n(){}[]-#*+?!/&$" );
		
		if( nextEnd && nextEnd <= end ) // Fond something
		{
			startPos = curPos;
			endPos = nextEnd;
			nextEnd++;
		}
		else
		{
			startPos = curPos;
			endPos = end;
		}
		
		if( startPos < endPos )
		{
			std::list<int> lst = parseWordInternal( startPos , endPos );
			for( int i : lst )
				ret.push_back( startPos - txt + i );
		}
		
		curPos = nextEnd;
		
	}while( *curPos && curPos <= end );
	
	return ret;
}

enum parserState
{
	Pstart
	, PvocaleOcc
	, PconsonantOcc
	, Pend
};

std::list<int> syllableParser::parseWordInternal( const char* startPos , const char* endPos )
{
	parserState state = Pstart;
	
	// List of syllable-separators
	std::list<int> positions;
	
	// Temporary value that will be filled with the position of the next syllable, if requested
	int syllablePos = -1;
	
	const char* curPos = startPos;
	
	while( true )
	{
		// Prevent overflow
		if( curPos >= endPos )
			state = Pend;
		
		if( syllablePos != -1 )
		{
			positions.push_back( syllablePos );
			syllablePos = -1;
		}
		//printf("%c ->%d\n",*curPos,state);
		
		switch( state )
		{
			case Pstart:
			{
				unsigned int size;
				if( ( size = isCombinStart( curPos ) ) )
					state = PconsonantOcc;
				else if( ( size = isDiphtong( curPos ) ) )
					state = PvocaleOcc;
				else if( ( size = isVocale( *curPos ) ) )
					state = PvocaleOcc;
				else
				{
					state = PconsonantOcc;
					size = 1;
				}
				curPos += size;
				break;
			}
			case PvocaleOcc: // vocal was last: find consonant!
			{
				unsigned short size;
				if(
					( size = isCombinVocalBefore( curPos ) )
					&& !( // Check if this is not the start of the next syllable
						isVocale( *(curPos + size) )
						&& isCombinStart( curPos )
						)
				)
				{
					state = Pstart;
					if( curPos + size + 1 < endPos )
						syllablePos = curPos - startPos + size - 1;
					
				}
				else if( ( size = isCombinStart( curPos ) ) && !isConsonant( *(curPos + size) ) )
				{
					state = PconsonantOcc;
					if( curPos + size + 1 < endPos )
						syllablePos = curPos - startPos - 1;
				}
				else if( ( size = isConsonant( *curPos ) ) )
				{
					if( isConsonant( *(curPos+1) ) ) // 1 Lookup to maybe separate between two consonants (e.g. 'karten')
						state = PvocaleOcc;
					else
					{
						state = Pstart;
						if( curPos + 1 < endPos )
							syllablePos = curPos - startPos - 1;
					}
				}
				else if( ( size = isVocale( *curPos ) ) )
				{
					if( *curPos != *(curPos-1) ) // same vocal
						syllablePos = curPos - startPos - 1;
					state = PvocaleOcc;
				}
				else
				{
					//const char* c2 = "ß";
					//printf("/// %d",*c2 == 'ß');
					size = 1;
				}
				curPos += size;
				break;
			}
			case PconsonantOcc: // consonant was last
			{
				unsigned short size;
				if( ( size = isCombinStart( curPos ) ) )
				{
					syllablePos = curPos - startPos;
					state = PconsonantOcc;
				}
				else if( ( size = isDiphtong( curPos ) ) )
					state = PvocaleOcc;
				else if( ( size = isVocale( *curPos ) ) )
					state = PvocaleOcc;
				else
				{
					state = Pstart;
					//printf("Error: %d\n",curPos - startPos);
					size = 1;
				}
				curPos += size;
				break;
			}
			default:
			case Pend:
				return positions;
		}
	}
}