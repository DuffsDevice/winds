#include "_type/type.text.h"
#include "_type/type.system.h"

bool charisof( char c , char* txt )
{
	while( *txt )
	{
		if( *txt++ == c )
			return true;
	}
	
	return false;
}

void _text::wrap()
{
	this->linePositions.clear();
	
	if( !this->font || !this->font->valid() )
		return;
	
	_u32	idx = 0; // Current index in the string
	_length	lineWidth = 0; // Width in pixels of the current line
	_length	lastWordWidth = 0; // Width in pixels of the current line
	_u32	lastBreakIndex = 0;
	
	// Characters after which to break
	static char breakChars[] = " ,.-:;?!+=/" "\\";
	
	// Push start
	this->linePositions.push_back( 0 );
	
	for( const char ch : this->text )
	{
		idx++;
		
		// advance the current lineWidth by the width of the char to display
		if( this->font->isCharSupported( ch ) )
		{
			_u16 w = this->font->getCharacterWidth( ch ) + 1;
			lineWidth += w;
			lastWordWidth += w;
		}
		
		//printf("%c -> %d , %d\n",ch,lineWidth,lastWordWidth);
		
		// Check if ch is a character where we possibbly could break
		if( ch == '\n' )
		{
			this->linePositions.push_back( idx );
			
			// Reset current line width since we added a line break
			lineWidth = 0;
			lastWordWidth = 0;
			lastBreakIndex = idx;
		}
		else if( charisof( ch , breakChars ) )
		{
			lastBreakIndex = idx;
			lastWordWidth = 0;
			//printf("BreakIndex %d!\n",lastBreakIndex);
		}
		
		if( lineWidth >= this->width )
		{
			if( lastBreakIndex != this->linePositions.back() )
			{
				//printf("Overflow %d!\n",lastBreakIndex);
				this->linePositions.push_back( lastBreakIndex );
			}
			
			// Reset current line width since we added a line break
			lineWidth = lastWordWidth;
		}
		
		//_system::submit();
	}
	
	this->linePositions.push_back( idx ); // Push end
	
	this->needsRefreshFlag = true;
}