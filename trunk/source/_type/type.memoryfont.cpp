#include "_type/type.memoryfont.h"
#include "nds.h"

_memoryfont::_memoryfont( string nN , _char fc , _char lc , _u8 ht , _u8 mH , const _u16* data , const _u8* widths , const _u16* offsets , _u8 ms , _u8 sW ) :
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

_u16 _memoryfont::getCharacterWidth( _char character , _u8 fontSize ) const {
	if( character == '\n' )
		return 0;
	if( this->monospace )
		return this->monospace;
	if( character == ' ' )
		return this->spaceWidth;
	return this->charWidths[ character - this->firstChar ];
}

_u16 _memoryfont::isMonospace() const {
	return this->monospace;
}

bool _memoryfont::isCharSupported( _char ch ) const {
	return ch == ' ' || ch == '\n' || ( ch <= this->lastChar && ch >= this->firstChar );
}

bool _memoryfont::valid() const {
	return this->charData != nullptr;
}

_u16 _memoryfont::getHeight( _u8 fontSize ) const {
	return this->height;
}

_u16 _memoryfont::getAscent( _u8 fontSize ) const {
	return this->mainHeight;
}

_u16 _memoryfont::drawCharacter( _bitmap* dest , _coord posX , _coord posY , _char ch , _pixel color , _rect clip , _u8 fontSize ) const 
{	
	if( ch == '\n' ) //Line Break
		return 0;
	
	if( ch == ' ' ) // Whitespace
		return this->spaceWidth;
	
	int clipX1 = clip.x;
	int clipY1 = clip.y;
	int clipX2 = clip.getX2();
	int clipY2 = clip.getY2();
	
	// Fetch data
	const _u16* pixelData = this->charData + this->charOffsets[ ch - this->firstChar ];
	
	// Attributes
	_int pixelsPerRow = this->charWidths[ ch - this->firstChar ];
	
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
		int pixelsToSkip = ( clipY1 - posY ) * pixelsPerRow;
		
		// Determine halfwords and bits to skip
		swiDivMod( pixelsToSkip , 16 , &halfwordsToSkip , &bitsToSkip );
		
		posY = clipY1;
		pixelData += halfwordsToSkip;		// skip over a bunch of halfwords
		
		if( bitsToSkip )
		{
			curr = *pixelData++;
			mask = 0x8000 >> bitsToSkip;
		}
	}
	
	// Fetch the destination where to draw To
	_pixelArray bmp = dest->getBitmap( posX , posY );
	
	// Pixels to skip on the LEFT
	_int xOffset = max( 0 , clipX1 - posX );
	_int bitmapWidth = dest->getWidth();
	
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
			if ( curr & mask && curX <= width && curX >= xOffset )
				bmp[curX] = color;
			
			curX++;
			
			mask >>= 1;
			
		}while( --rowCount > 0 );
		
		// Move to next Line
		bmp += bitmapWidth;
		
	}while( --height > 0 );
	
	return this->monospace ? this->monospace : pixelsPerRow;
}