#include <parser.h>

unsigned char TinyHyphenator::getNumCharsBeforeSyllableEnd( Literal start ) const 
{
	unsigned char numConsonants = 0;
	Literal curChar = start;
	while( *curChar && isConsonant( curChar ) )
	{
		curChar++;
		numConsonants++;
	}
	
	// If there is no next vocal in the word, there will be no additional syllable
	if( !*curChar )
	{
		// If we return the number of consonants,
		// the syllable would be behind the word and would be ignored
		return numConsonants;
	}
	
	// Simple case: If we have less than two consonants, we definitely
	// need the ones that are there to start the next syllable
	if( numConsonants < 2 )
		return 0;
	
	// Check, whether and how long any possible indivisible element is
	bool indivisible = isIndivisible( start );
	
	// Standard case with two consonants
	if( numConsonants == 2 )
	{
		if( indivisible )
			return isSyllableStart( start ) ? 0 : 2;
		return 1;
	}
	
	// Allocate temporary memory
	char* temp = new char[numConsonants+1];
	
	// Search all adjacent consonants for a starting letter combination
	// The longest combination found wins,
	// if there is no shorter start-combination where the
	// left over characters make up an ending-combination
	int found = -1;
	int indivisibleStart = isIndivisible( start );
	
	for( int findOffset = 0 ; findOffset < numConsonants ; findOffset++ )
	{
		// A combination at the very end was found
		if( (numConsonants - findOffset) == isSyllableStart( &start[findOffset] ) )
		{
			if( found < 0 && findOffset >= indivisibleStart )
				found = findOffset;
			else if( findOffset > found + 1 ) // Check if it does fit better
			{
				// Copy to destination array
				strncpy( temp , start , findOffset );
				
				// Are the left over chars a syllable-end?
				if( isSyllableEnd( temp ) )
					found = findOffset;
			}
		}
	}
	
	// If there was no start-combination, look
	// how long the end-combination is (if there is one)
	if( found < 0 )
	{
		// Limit the characters to look in to numConsonants minus one,
		// because we definitely need one consonant, to start the next syllable
		int maxLen = numConsonants - 1;
		
		// Ignore the required starting-consonant, if the consonants cant be split up
		if( maxLen < indivisibleStart )
			maxLen = indivisibleStart;
		
		strncpy( temp , start , maxLen );
		
		// Check, whether and if yes, how long the ending-combination is
		if( !(found = isSyllableEnd( temp ) ) )
		{
			// If we could not fand any combination, we take all
			// but the last char to form the end of the current syllable
			found = numConsonants - 1;
		}
	}
	
	// Delete the newly allocated buffer
	delete[] temp;
	
	return found;
}

std::list<int> TinyHyphenator::parseTextInternal( Literal txt , Literal end ) const 
{
	Literal startPos;
	Literal endPos;
	
	// Current poition of iteration
	Literal curPos = txt;
	
	std::list<int> ret;
	
	do
	{
		Literal nextEnd = TinyHyphenator::findFirstOf( curPos , language.wordSeparators );
		
		if( nextEnd && nextEnd <= end ){ // Found something
			startPos = curPos;
			endPos = nextEnd;
			nextEnd++;
		}
		else{
			startPos = curPos;
			endPos = end;
		}
		
		if( startPos < endPos )
		{
			std::list<int> lst = TinyHyphenator::parseWordInternal( startPos , endPos );
			
			std::ptrdiff_t delta = startPos - txt;
			
			for( int& i : lst )
				i+= delta;
			
			ret.splice(ret.end(), lst);
		}
		
		curPos = nextEnd;
		
	}while( *curPos && curPos <= end );
	
	return ret;
}

enum parserState
{
	Pstart
	, PvowelOcc
	, PconsonantOcc
	, Pend
};

std::list<int> TinyHyphenator::parseWordInternal( Literal startPos , Literal endPos ) const 
{
	parserState state = Pstart;
	
	// List of syllable-separators
	std::list<int> positions;
	
	// Temporary value that will be filled with the position of the next syllable, if requested
	int syllablePos = -1;
	
	Literal curPos = startPos;
	unsigned int size = 0;
	int index = 0;
	
	while( true )
	{
		// Prevent overflow
		if( curPos >= endPos )
			state = Pend;
		
		if( syllablePos != -1 ){
			if( syllablePos < endPos - startPos - 2 )
				positions.push_back( syllablePos );
			syllablePos = -1;
		}
		
		switch( state )
		{
			case Pstart:
				// Set the state dependent of what type of char there is
				if( ( size = isSyllableStart( curPos ) ) || ( size = isConsonant( curPos ) ) )
					state = PconsonantOcc;
				else if( ( size = isDiphtong( curPos ) ) || ( size = isVowel( curPos ) ) )
					state = PvowelOcc;
				else
					state = Pstart;
				break;
			case PvowelOcc:
				if( ( size = isDiphtong( curPos ) ) || ( size = isVowel( curPos ) ) )
				// We have a second vocal after the last character -> break between them
					syllablePos = index - 1;
				else if( ( size = isConsonant( curPos ) ) )
				{
					// We have an unknown number of consonants ahead
					// -> determine the start of the next syllable
					size = getNumCharsBeforeSyllableEnd( curPos );
					syllablePos = index + size - 1;
					state = Pstart;
				}
				else
					state = Pstart;
				break;
			case PconsonantOcc:
				// The only thing that could possibly appear after a/some consonant(s) is a vocal!
				if( ( size = isDiphtong( curPos ) ) || ( size = isVowel( curPos ) ) )
					state = PvowelOcc;
				else
					state = Pstart;
				break;
			default:
			case Pend:
				return positions;
		}
		
		// Initialize next iteration cycle
		
		if( !size )// Step at least one char ahead
			size = 1;
		index += size;
		curPos += size;
		size = 0;
	}
}