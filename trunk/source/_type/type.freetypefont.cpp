#include "_type/type.freetypefont.h"
#include "_type/type.system.h"

_freetypefont::_freetypefont( string path , const u8 width , const u8 height ) :
	_font( "Freetype" )
	, faceID( 0 )
	, charMapIndex( 0 )
{
	
	this->faceID = _system::_faceTypeManager_->addFace( path , 0 );
	//printf("Hello:%p",_system::_faceTypeManager_);while(true);
	this->scaler.face_id = (FTC_FaceID) this->faceID;
	this->scaler.width = (FT_UInt) width;
	this->scaler.height = (FT_UInt) height;
	this->scaler.pixel = 10; // DPI
	this->scaler.x_res = 0;  // Computations are done in pixels and not real units
	this->scaler.y_res = 0;  // Computations are done in pixels and not real unit
	getSbit(' '); 		// Load the ' ' blank char...no bitmap, no drawing, but
						// metrics, fastest loading
}
void _freetypefont::getSbit( const _char codepoint ) const 
{
	_system::_faceTypeCache_->loadSbit( this->scaler, this->charMapIndex, codepoint, (FTC_SBit *) &this->sbit);
}

_u16 _freetypefont::getCharacterWidth( const _char codepoint ) const 
{ 
	getSbit(codepoint);
	return (_u16) sbit->xadvance;
}

_u16 _freetypefont::isMonospace() const 
{ 
	return false;
}

_u16 _freetypefont::getHeight() const 
{ 
	return (_u16) this->sbit->height;
}

_u16 _freetypefont::getMainHeight() const 
{ 
	return (_u16) this->sbit->height/3*2;
}

bool _freetypefont::valid() const 
{ 
	return this->faceID != nullptr;
}

bool _freetypefont::isCharSupported( const _char codepoint ) const { 
	getSbit( codepoint );
	if ( ( this->sbit->buffer == 0) || ( this->sbit->width == 0 ) || ( this->sbit->height == 0 ) )
		return false;
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
0x20, 0x40, 0x80, 0x300, 0xC00, 0x3000, 0x1C000, 0xE0000, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFFFFFFFE};

_u16 _freetypefont::drawCharacter( _bitmap* dest , _coord x , _coord y , _char letter , _pixel color , _rect clip ) const 
{
	getSbit( letter );

	_u8 fontWidth = this->sbit->width;
	_u8 fontHeight = this->sbit->height;
	const _u16 output = this->sbit->xadvance;

	// No need to draw blank or glyphs missing a bitmap
	switch (letter) {
		case 10:  // Line feed
		case 13:  // Carriage return
			return output;
		default:
			break;
	}
	if (!this->sbit->buffer) return output;

	_u8 maxGrays = this->sbit->max_grays;
	if (maxGrays == 0) return output;

	// offsets for the glyph
	x += this->sbit->left;
	y -= this->sbit->top - this->scaler.height;

	// no need to blit to screen
	if ( y > clip.getY2() ) return output;
	if ( y + fontHeight < clip.getY() ) return output;
	if ( x > clip.getX2() ) return output;
	if ( x + fontWidth < clip.getX() ) return output;

	// Calculate values for clipping
	_u16 startX = max( x , clip.getX() );
	_u16 endX = min( x , clip.getX() );
	_u16 startY = max( y , clip.getY() );
	_u16 endY = min( y , clip.getY() );

	// Calculate clipping starting offsets - tells us the pixel offset
	// from the top-left of the character that we start drawing from
	_u16 offsetStartX = x > clip.getX() ? 0 : clip.getX() - x;
	_u16 offsetStartY = y > clip.getY() ? 0 : clip.getY() - y;

	// Calculate dimensions of region to copy
	_u16 clipWidth = (endX - startX) + 1;
	_u16 clipHeight = (endY - startY) + 1;

	// Ensure region to be drawn does not exceed the size of the character
	if (clipWidth > fontWidth - offsetStartX) clipWidth = fontWidth - offsetStartX;
	if (clipHeight > fontHeight - offsetStartY) clipHeight = fontHeight - offsetStartY;

	// Abort if there is no copying to be done
	if ((clipWidth == 0) || (clipHeight == 0)) return output;

	// Copy the pixel data
	_u8 grayLevel;
	_u8* src = this->sbit->buffer;
	_pixel rgb;
	_pixel bitmapColour;
	
	// This is the colour of the font
	_u16 r = color & 0x7C00;
	_u16 g = color & 0x03E0;
	_u16 b = color & 0x001F;
	
	for (u16 pY = 0; pY < clipHeight; ++pY) {              
		for (u16 pX = 0; pX < clipWidth; ++pX) {  
		
			grayLevel = *(src+ (offsetStartY+pY) * fontWidth+ offsetStartX+pX);
			
			if (grayLevel > 0) {
				
				// We do nothing for most of a bitmap (blank pixels)
				if (grayLevel == maxGrays) {
					// We just need to copy the few opaque pixels 
					dest->drawPixel(x + offsetStartX + pX, y + offsetStartY + pY, color);
				} else {
				
					// Antialiasing for the many gray pixels
					// We get the existing colour 
					bitmapColour = dest->getPixel(x + offsetStartX + pX, y + offsetStartY + pY);
					
					// Mix it with the font colour with regards to the
					// grayLevel (opacity/transparency)making the
					// transparency colour unnecessary
					
					// This is good enough for now: 
					
					// Needs 1kb to hold the transparencyJump array
					// (or 512b with one more test)
					u32 jump = transparencyJump[grayLevel];
					rgb = bitmapColour;
					
					s16 increment = r - (bitmapColour & 0x7C00);
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
					
					dest->drawPixel(x + offsetStartX + pX, y + offsetStartY + pY, rgb);
				}                            
			}
		}
	}
		
	return output;
}
