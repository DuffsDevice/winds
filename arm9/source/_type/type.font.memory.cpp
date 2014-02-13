#include "_type/type.font.memory.h"
#include "nds.h"

_memoryFont::_memoryFont( string nN , _char fc , _char lc , _u8 ht , _u8 mH , const _u16* data , const _u8* widths , const _u16* offsets , _u8 ms , _u8 sW ) :
	_font( nN )
	, spaceWidth( sW )
	, monospace( ms )
	, firstChar( fc )
	, lastChar( lc )
	, height( ht )
	, mainHeight( mH )
	, charData( data )
	, charWidths( widths )
	, charOffsets( offsets )
{ }

_length _memoryFont::getCharacterWidth( _char character , _u8 fontSize ) const
{
	// Check if 'character' is a space
	if( character == ' ' )
		return this->spaceWidth;
	// Check if the font supports the char
	else if( character > this->lastChar || character < this->firstChar)
		return 0;
	// Check if it really matters to do the lookup
	else if( this->monospace )
		return this->monospace;
	
	// Lookup
	return this->charWidths[ character - this->firstChar ];
}

_length _memoryFont::drawCharacter( _pixelArray dest , _length destWidth , _coord posX , _coord posY , _char ch , _color color , _rect clip , _u8 fontSize ) const 
{
	// If it's a Whitespace
	if( ch == ' ' )
		return this->spaceWidth;
	
	// Attributes
	if( ch > this->lastChar || ch < this->firstChar)
		return 0;
	
	_int pixelsPerRow = this->charWidths[ ch - this->firstChar ];
	
	int clipX1 = clip.x;
	int clipY1 = clip.y;
	int clipX2 = clip.getX2();
	int clipY2 = clip.getY2();
	
	// Fetch data
	const _u16* pixelData = this->charData + this->charOffsets[ ch - this->firstChar ];
	
	_u16 curr = 0;
	_u16 mask = 0; // setting mask to 0 forces read of next word

	_int height = min( (_int)getHeight() , clipY2 - posY + 1 );
	_int width = min( pixelsPerRow , clipX2 - posX + 1 );
	
	if( height <= 0 || width <= 0 )
		return pixelsPerRow;

	// skip over font data corresponding to pixels ABOVE the clipping rectangle.
	if( posY < clipY1 )
	{
		int bitsToSkip;
		int halfwordsToSkip;
		int rowsToSkip = ( clipY1 - posY );
		int pixelsToSkip = rowsToSkip * pixelsPerRow;
		
		// Determine halfwords and bits to skip
		swiDivMod( pixelsToSkip , 16 , &halfwordsToSkip , &bitsToSkip );
		
		posY = clipY1;
		dest += rowsToSkip * destWidth;
		pixelData += halfwordsToSkip;		// skip over a bunch of halfwords
		height -= rowsToSkip;
		
		if( bitsToSkip )
		{
			curr = *pixelData++;
			mask = 0x8000 >> bitsToSkip;
		}
	}
	
	// Pixels to skip on the LEFT
	_int xOffset = max( 0 , clipX1 - posX );
	
	do
	{
		_u16 rowCount = pixelsPerRow;
		_int curX = 0;
		
		do
		{
			// if we have runout, get next chunk
			if ( !mask )
			{
				mask = 0x8000;
				curr = *pixelData++;
			}
			
			// unpack next pixel
			// if we need to, write it to the bitmap
			if ( curr & mask && curX < width && curX >= xOffset )
				dest[curX] = color;
			
			curX++;
			
			mask >>= 1;
			
		}while( --rowCount > 0 );
		
		// Move to next Line
		dest += destWidth;
		
	}while( --height > 0 );
	
	return this->monospace ? this->monospace : pixelsPerRow;
}