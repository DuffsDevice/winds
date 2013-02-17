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

_u16 _memoryfont::drawCharacter( _bitmap* dest , _coord x0 , _coord y0 , _char ch , _pixel color , _rect clip , _u8 fontSize ) const 
{
	//if( ch == ' ' )
	//	return this->spaceWidth;
	//int s = cpuGetTiming();
	//
	////startTimer( reinterpret_cast<void*>(&_memoryfont::drawCharacter) );
	//
	//if( !isCharSupported( ch ) )
	//	ch = '?';
	//
	//const _bit *data 	= this->charData + this->charOffsets[ ch - this->firstChar ];
	//_length width 		= this->getCharacterWidth( ch );
	//
	//// Check for transparent
	//if( !RGB_GETA(color) )
	//	goto end;
	//
	//// no need to blit to screen
	//if ( y0 > clip.getY2() || y0 + this->height < clip.y || x0 > clip.getX2() || x0 + width < clip.x ) return width;
	//
	//if( this->monospace && width <= this->monospace - 2 )
	//{
	//	x0++;
	//	if( width <= this->monospace - 4 )
	//		x0++;
	//}
	//
	//for( int x = 0; x != width ; ++x ){
	//	for( int y = 0; y != this->height ; y++ ){
	//		if( data[ y * width + x ] )
	//			dest->drawPixel( x0 + x , y0 + y , color );
	//	}
	//}
	//
	//end: // Breakpoint
	//if( this->monospace != 0 )
	//	return this->monospace;
	
	if( ch == '\n' ) //Line Break
		return 0;
	
	if( ch == ' ' ) // Whitespace
		return this->spaceWidth;
	
	int clipX1 = clip.x;
	int clipY1 = clip.y;
	int clipX2 = clip.getX2();
	int clipY2 = clip.getY2();
	
	const _u16* pixelData = this->charData + this->charOffsets[ ch - this->firstChar ];
	
	_int pixelsPerRow = this->charWidths[ ch - this->firstChar ];
	
	// Abort if there is nothing to render
	if ((clipY2 < y0) ||
		(clipY1 > y0 + this->height - 1 ) ||
		(x0 > clipX2) ||
		(x0 + pixelsPerRow - 1 < clipX1))
			return pixelsPerRow;

	// If no colour is specified, default to black
	if (!color) color = COLOR_BLACK;
	
	_u16 curr;
	_u16 mask;

	// adjust clipY2 to be the last row in the glyph
	// so we only write while (y=<clipY2)
	if (clipY2 > y0 + getHeight() - 1) {
		clipY2 = y0 + getHeight() - 1;
	}

	// setting mask to 0 forces read of next word. setting curr
	// avoids a compiler warning.
	mask = 0;
	curr = 0;

	// skip over font data corresponding to pixels ABOVE
	// the clipping rectangle.
	if (y0 < clipY1) {
		u16 rowsToSkip = clipY1-y0;
		u16 pixelsToSkip = rowsToSkip * pixelsPerRow;
		u16 bitsToSkip = pixelsToSkip & 15;
		y0 = clipY1;
		pixelData += pixelsToSkip / 16;		// skip over a bunch of u16s
		if (bitsToSkip) {
			curr = *pixelData++;
			mask = 0x8000 >> bitsToSkip;
		}
	}

	while (y0 <= clipY2) {
		u16 rowCount = pixelsPerRow;
		u16 rowX = x0;
		while (rowCount-- > 0) {
			// if we have runout, get next chunk
			if (!mask) {
				mask = 0x8000;
				curr = *pixelData++;
			}
			
			// unpack next pixel
			if (curr & mask) {
				// if we need to, write it to the bitmap
				if (
					rowX >= clipX1 && rowX <= clipX2	// not clipped X-wise
				) {
					dest->drawPixel(rowX, y0, color);
				}
			}
			mask >>= 1;
			rowX++;
		}
		y0++;
	}
	
	//stopTimer( reinterpret_cast<void*>(&_memoryfont::drawCharacter) );
	//printf("Timing for %c: %d\n",ch,cpuGetTiming() - s );
	//while(true);
	
	if( this->monospace )
		return this->monospace;
	
	return pixelsPerRow;
}