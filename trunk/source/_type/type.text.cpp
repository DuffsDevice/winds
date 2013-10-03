#include "_type/type.text.h"
#include "_type/type.system.h"
#include "_library/syllable.h"

void _text::wrap()
{
	this->linePositions.clear();
	
	if( !this->font || !this->font->isValid() )
		return;
	
	_u32	idx = 0; // Current index in the string
	_length	lineWidth = 0; // Width in pixels of the current line
	_length	lastWordWidth = 0; // Width in pixels of the current line
	_u32	lastBreakIndex = 0;
	
	// Characters after which to break
	static _char breakChars[] = " ,.-:;?!+=/" "\\";
	
	// Push start
	this->linePositions.push_back( 0 );
	
	const _char* iteratorText = this->text.c_str();
	const _char* iteratorTextEnd = iteratorText + strlen( iteratorText );
	
	for( const _char* ch = iteratorText ; ch < iteratorTextEnd ; ch++ )
	{
		idx++;
		
		// Check if ch is a character where we possibbly could break
		if( *ch == '\n' )
		{
			this->linePositions.push_back( idx );
			
			// Reset current line width since we added a line break
			lineWidth = 0;
			lastWordWidth = 0;
			lastBreakIndex = idx;
		}
		if( this->font->isCharSupported( *ch ) )
		// advance the current lineWidth by the width of the _char to display
		{
			_u16 w = this->font->getCharacterWidth( *ch ) + 1;
			lineWidth += w;
			lastWordWidth += w;
		}
		
		// Index we have a possible break here
		if( charisof( *ch , breakChars ) )
		{
			lastBreakIndex = idx;
			lastWordWidth = 0;
		}
		
		if( lineWidth >= this->width )
		{
			const _char* text = iteratorText + lastBreakIndex;
			const _char* end = text + idx - lastBreakIndex + 1;
			_list<int> lst = syllableParser::parseText( text , end );
			
			if( lst.size() )
			{
				this->linePositions.push_back( ( 1 << 31 ) | ( lastBreakIndex + lst.back() + 1 ) );
				
				// Reset current line width since we added a line break
				string str = text + lst.back();
				str.resize( end - text - lst.back() );
				lineWidth = this->font->getStringWidth( str );
				
				lastWordWidth = 0;
			}
			else 
			{
				if( lastBreakIndex != this->linePositions.back() )
					this->linePositions.push_back( lastBreakIndex );
				
				lineWidth = lastWordWidth;
			}
		}
	}
	
	this->linePositions.push_back( idx ); // Push end
	
	this->needsRefreshFlag = true;
}