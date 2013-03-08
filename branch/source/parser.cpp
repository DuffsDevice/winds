#include "parser.h"

const char* findFirstOf( const char* text , const char* match )
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

std::list<int> parser::parseTextInternal( const char* txt )
{
	const char* startPos;
	const char* endPos;
	// Current poition of iteration
	const char* curPos = txt;
	
	std::list<int> ret;
	
	do
	{
		const char* nextEnd = findFirstOf( curPos , " ,\n(){}[]-#*+?!/&$" );
		
		if( nextEnd ) // Fond something
		{
			startPos = curPos;
			endPos = nextEnd;
			nextEnd++;
		}
		else
		{
			startPos = curPos;
			endPos = startPos + strlen( startPos );
		}
		
		if( startPos < endPos )
		{			
			std::list<int>&& lst = parseWordInternal( startPos , endPos );
			for( int i : lst )
				ret.push_back( startPos - txt + i );
		}
		
		curPos = nextEnd;
		
	}while( *curPos );
	
	return ret;
}

unsigned char parser::isDiphtong( const char* txt )
{
	const char* curExpr = diphthongs;
	
	do
	{
		unsigned int i = strlen( curExpr );
		
		if( strncmp( curExpr , txt , i ) == 0 )
			return i;
		
		curExpr += i + 1;
		
	}while( *curExpr );
	
	return 0;
}

unsigned char parser::isCombinStart( const char* txt )
{
	const char* curExpr = combinStart;
	
	do
	{
		unsigned int i = strlen( curExpr );
		
		if( strncmp( curExpr , txt , i ) == 0 )
			return i;
		
		curExpr += i + 1;
		
	}while( *curExpr );
	
	return 0;
}

unsigned char parser::isCombinVocalBefore( const char* txt )
{
	const char* curExpr = combinVocalBefore;
	
	do
	{
		unsigned int i = strlen( curExpr );
		
		if( strncmp( curExpr , txt , i ) == 0 )
			return i;
		
		curExpr += i + 1;
		
	}while( *curExpr );
	
	return 0;
}

enum parserState
{
	Pstart
	, PvocaleOcc
	, PconsonantOcc
	, Pend
};

std::list<int> parser::parseWordInternal( const char* startPos , const char* endPos )
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
					printf("Error: %d\n",curPos - startPos);
					size = 1;
				}
				curPos += size;
				break;
			}
			default:
				return positions;
		}
	}
}