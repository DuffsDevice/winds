#include "_type/type.freetypefont.h"

#define STB_TRUETYPE_IMPLEMENTATION // Request implementation instead of just header
#include "_library/truetype.h"
#undef STB_TRUETYPE_IMPLEMENTATION

_u32 LUT_Transparency[256] = 
{
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20, 
	0x0, 0x40008000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20, 0x0, 
	0x0, 0x80000, 0x40008000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20000, 0x20, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x42108420, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x400, 0x4002000, 0x20, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x80000, 0x0, 0x40008000, 0x0, 0x0, 0x40, 0x0, 0x10204080, 0x0, 0x20, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x10000, 0x0, 0x0, 0x49249248, 0x0, 0x0, 0x20000, 0x800, 0x20, 0x0, 0x2000, 0x40800, 0x80, 0x0, 
	0x0, 0x20000, 0x0, 0x80000, 0x800, 0x0, 0x42108420, 0x0, 0x8000, 0x20, 0x0, 0x80000, 0x4000, 0x0, 0x0, 0x800, 
	0x0, 0x0, 0x0, 0x100, 0x40800, 0x1000, 0x4002000, 0x40008000, 0x20, 0x80, 0x800, 0x8000, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0xA0000, 0x2000, 0x200, 0x20, 0x40008000, 0x10000, 0x0, 0x0, 0x80000, 0x20000, 0x804200, 0x2000, 
	0x0, 0x40000, 0x10204080, 0x0, 0x80, 0x1001000, 0x20020, 0x400, 0x0, 0x42108420, 0x0, 0x10000, 0x40, 0x4002000, 0x200, 0x0, 
	0x0, 0x8000, 0x40000, 0x0, 0x10004000, 0x80020, 0x100, 0x4000, 0x0, 0x0, 0x49249248, 0x0, 0x0, 0x80000, 0x2400, 0x80, 
	0x20000, 0x4002000, 0x800, 0x4010400, 0x8020, 0x81000, 0x0, 0x0, 0x2000, 0x20000, 0x40800, 0x44008000, 0x80, 0x8800, 0x0, 0x0, 
	0x0, 0x0, 0x22000, 0x220, 0x40000, 0x0, 0x80000, 0x804200, 0x800, 0x10004000, 0x1080, 0x20000, 0x42108420, 0x0, 0x84000, 0x200, 
	0x8000, 0x40800, 0x10020, 0x800, 0x0, 0x41041040, 0x80000, 0x2080, 0x4002000, 0x100, 0x8000, 0x10204080, 0x20000, 0x40408000, 0x800, 0x0, 
	0x0, 0x2020, 0x4000, 0x804200, 0x10000, 0x20080, 0x4010400, 0x80200, 0x40800, 0x800, 0x1001000, 0x2000, 0x4002000, 0x18004000, 0x60008000, 0x80000, 
	0x20, 0x40, 0x80, 0x300, 0xC00, 0x3000, 0x1C000, 0xE0000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFFFFFFFE
};

_freetypefont::_freetypefont( string path ) :
	_direntry( path )
	, _font( _direntry::getName() )
	, cache( nullptr )
	, cacheSize( this->getSize() )
{
	if( !this->isExisting() || !this->cacheSize )
		return;
	
	this->cache = new _u8[ this->cacheSize ];
	
	if( !this->read( this->cache , this->cacheSize ) )	
	{
		delete[] cache;
		this->cache = 0;
		return;
	}
	
	stbtt_InitFont( &this->fontInfo , this->cache , stbtt_GetFontOffsetForIndex( this->cache , 0 ) );
}


_freetypefont::~_freetypefont()
{
	delete[] cache;
	this->cache = 0;
}

_length _freetypefont::getCharacterWidth( const _char codepoint , _u8 fontSize ) const 
{ 
	int advWidth , leftSideBearing;
	stbtt_GetCodepointHMetrics( &this->fontInfo , codepoint , &advWidth , &leftSideBearing );
	return stbtt_ScaleForPixelHeight( &this->fontInfo , fontSize ) * advWidth;
}

_length _freetypefont::isMonospace() const 
{ 
	return false;
}

bool _freetypefont::valid() const 
{
	//! TODO: Implement
	return true;
}

bool _freetypefont::isCharSupported( const _char codepoint ) const { 
	/*getSbit( codepoint );
	if ( ( this->sbit->buffer == 0) || ( this->sbit->width == 0 ) || ( this->sbit->height == 0 ) )
		return false;*/
	return true;
};

_u32 transparencyJump[256] =  {
	0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20, 
	0x0, 0x40008000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20, 0x0, 
	0x0, 0x80000, 0x40008000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20000, 0x20, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x42108420, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x400, 0x4002000, 0x20, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x80000, 0x0, 0x40008000, 0x0, 0x0, 0x40, 0x0, 0x10204080, 0x0, 0x20, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x10000, 0x0, 0x0, 0x49249248, 0x0, 0x0, 0x20000, 0x800, 0x20, 0x0, 0x2000, 0x40800, 0x80, 0x0, 
	0x0, 0x20000, 0x0, 0x80000, 0x800, 0x0, 0x42108420, 0x0, 0x8000, 0x20, 0x0, 0x80000, 0x4000, 0x0, 0x0, 0x800, 
	0x0, 0x0, 0x0, 0x100, 0x40800, 0x1000, 0x4002000, 0x40008000, 0x20, 0x80, 0x800, 0x8000, 0x0, 0x0, 0x0, 0x0, 
	0x0, 0x0, 0x0, 0x0, 0xA0000, 0x2000, 0x200, 0x20, 0x40008000, 0x10000, 0x0, 0x0, 0x80000, 0x20000, 0x804200, 0x2000, 
	0x0, 0x40000, 0x10204080, 0x0, 0x80, 0x1001000, 0x20020, 0x400, 0x0, 0x42108420, 0x0, 0x10000, 0x40, 0x4002000, 0x200, 0x0, 
	0x0, 0x8000, 0x40000, 0x0, 0x10004000, 0x80020, 0x100, 0x4000, 0x0, 0x0, 0x49249248, 0x0, 0x0, 0x80000, 0x2400, 0x80, 
	0x20000, 0x4002000, 0x800, 0x4010400, 0x8020, 0x81000, 0x0, 0x0, 0x2000, 0x20000, 0x40800, 0x44008000, 0x80, 0x8800, 0x0, 0x0, 
	0x0, 0x0, 0x22000, 0x220, 0x40000, 0x0, 0x80000, 0x804200, 0x800, 0x10004000, 0x1080, 0x20000, 0x42108420, 0x0, 0x84000, 0x200, 
	0x8000, 0x40800, 0x10020, 0x800, 0x0, 0x41041040, 0x80000, 0x2080, 0x4002000, 0x100, 0x8000, 0x10204080, 0x20000, 0x40408000, 0x800, 0x0, 
	0x0, 0x2020, 0x4000, 0x804200, 0x10000, 0x20080, 0x4010400, 0x80200, 0x40800, 0x800, 0x1001000, 0x2000, 0x4002000, 0x18004000, 0x60008000, 0x80000, 
	0x20, 0x40, 0x80, 0x300, 0xC00, 0x3000, 0x1C000, 0xE0000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFFFFFFFE
};

_length _freetypefont::drawCharacter( _pixelArray dest , _length bitmapWidth , _coord x , _coord y , _char letter , _pixel color , _rect clip , _u8 fontSize ) const 
{	
	int fontWidth , fontHeight , xOffset , yOffset , ascent /*, nextCharBegin*/ /*, xOffset2*/;
	
	// Get Scale
	float scale = stbtt_ScaleForPixelHeight( &this->fontInfo , fontSize );
	
	stbtt_GetFontVMetrics( &this->fontInfo , &ascent , 0 , 0 );
	
	_u8* bitmap = stbtt_GetCodepointBitmap( &this->fontInfo , 0 , scale , letter , &fontWidth , &fontHeight , &xOffset , &yOffset );
	
	_length output = fontWidth;
	
	// In case of error
	if( !bitmap )
		return output;
	
	// No need to draw blank or glyphs missing a bitmap
	switch (letter) {
		case 10:  // Line feed
		case 13:  // Carriage return
			STBTT_free( bitmap , 0 ); // 2nd arg unused
			return output;
		default:
			break;
	}

	// offsets for the glyph
	x += xOffset;
	y += ascent * scale + yOffset;

	// Check if necessary to draw
	if ( 
		y > clip.getY2()
		|| y + fontHeight < clip.y
		|| x > clip.getX2()
		|| x + fontWidth < clip.x
	){
		STBTT_free( bitmap , 0 ); // 2nd arg unused
		return output;
	}

	// Calculate values for clipping
	_u16 startX = max( x , clip.x );
	_u16 endX = min( x , clip.x );
	_u16 startY = max( y , clip.y );
	_u16 endY = min( y , clip.y );

	// Calculate clipping starting offsets - tells us the pixel offset
	// from the top-left of the character that we start drawing from
	_u16 offsetStartX = x > clip.x ? 0 : clip.x - x;
	_u16 offsetStartY = y > clip.y ? 0 : clip.y - y;

	// Calculate dimensions of region to copy
	_u16 clipWidth = (endX - startX) + 1;
	_u16 clipHeight = (endY - startY) + 1;

	// Ensure region to be drawn does not exceed the size of the character
	if (clipWidth > fontWidth - offsetStartX)
		clipWidth = fontWidth - offsetStartX;
	
	if (clipHeight > fontHeight - offsetStartY)
		clipHeight = fontHeight - offsetStartY;

	// Abort if there is no copying to be done
	if ( !clipWidth || !clipHeight )
	{
		STBTT_free( bitmap , 0 ); // 2nd arg unused
		return output;
	}

	// Copy the pixel data
	_u8		grayLevel;
	_pixel	rgb;
	_pixel	bitmapColour;
	
	// This is the colour of the font
	_u16 r = color & 0x7C00;
	_u16 g = color & 0x03E0;
	_u16 b = color & 0x001F;
	
	for ( _u16 pY = 0 ; pY < clipHeight ; ++pY )
	{              
		for ( _u16 pX = 0 ; pX < clipWidth ; ++pX )
		{  
			grayLevel = bitmap[ ( offsetStartY + pY ) * fontWidth + offsetStartX + pX ];
			
			if (grayLevel > 0)
			{
				
				// We do nothing for most of a bitmap (blank pixels)
				if (grayLevel == 255)
				{
					// We just need to copy the few opaque pixels 
					_int drawY = y + offsetStartY + pY;
					_int drawX = x + offsetStartX + pX;
					
					dest[ drawX + drawY * bitmapWidth ] = color;
				}
				else
				{
					_int drawY = y + offsetStartY + pY;
					_int drawX = x + offsetStartX + pX;
					
					// Antialiasing for the many gray pixels
					// We get the existing colour 
					bitmapColour = dest[ drawX + drawY * bitmapWidth ];
					
					// Mix it with the font colour with regards to the
					// grayLevel (opacity/transparency)making the
					// transparency colour unnecessary
					
					// This is good enough for now: 
					
					// Needs 1kb to hold the transparencyJump array
					// (or 512b with one more test)
					_u32 jump = transparencyJump[grayLevel];
					rgb = bitmapColour;
					
					_s16 increment = r - (bitmapColour & 0x7C00);
					if (increment >= 0) {
						rgb += (((increment * grayLevel) >> 8) & 0x7C00);
						rgb += (jump & BIT(increment)) ? 0x0400 : 0;
					} else {
						rgb -= (((-increment * grayLevel) >> 8) & 0x7C00);
						rgb -= (jump & BIT(-increment)) ? 0x0400 : 0;                                
					}
					
					increment = g - (bitmapColour & 0x03E0);
					if (increment >= 0) {
						rgb += (((increment * grayLevel) >> 8) & 0x03E0); 
						rgb += (jump & BIT(increment)) ? 0x0020 : 0;
					} else {
						rgb -= (((-increment * grayLevel)>>8) & 0x03E0);
						rgb -= (jump & BIT(-increment)) ? 0x0020 : 0;                                
					}
					
					increment = b - (bitmapColour & 0x001F);
					if (increment >= 0) {
						rgb += (((increment * grayLevel) >> 8) & 0x001F);
						rgb += (jump & BIT(increment)) ? 0x0001 : 0;
					} else {
						rgb -= (((-increment * grayLevel) >> 8) & 0x001F);
						rgb -= (jump & BIT(-increment)) ? 0x0001 : 0;                                
					}					
					
					dest[ drawX + drawY * bitmapWidth ] = rgb;
				}                            
			}
		}
	}
	
	return output;
}

_length _freetypefont::getAscent( _u8 fontSize ) const 
{
	float scale = stbtt_ScaleForPixelHeight( &this->fontInfo , fontSize );
	
	int ascent;
	stbtt_GetFontVMetrics( &this->fontInfo , &ascent , 0 , 0 );
	
	return scale * ascent + 0.5;
}