#include <_type/type.bitmap.h>
#include <_func/func.gradientcreator.h>

#include <nds/arm9/math.h>
#include <nds/bios.h>

void _bitmap::setWidth( _length w )
{
	_codeAnalyzer analyzer {"_bitmap::setWidth"};
	
	// limit
	w = max( w , _length(1) );
	
	if( this->width == w )
		return;
	
	if( this->isValid() )
	{
		// Temp...
		_pixelArray newBmp;
		
		if( this->wasAllocated )
		{
			newBmp = new _pixel[ w * this->height ];
			memset16( newBmp , 0 , w * this->height );
		}
		else
			newBmp = this->bmp;
			
		if( w < this->width )
		{
			for( _int y = 1; y != this->height ; y++ )
				for( _int x = 0; x != w ; x++ )
					newBmp[ y * w + x ] = this->bmp[ y * this->width + x ];
		}
		else
		{
			for( _int y = this->height ; y > 0 ; y-- )
				for( _int x = this->width - 1; x >= 0 ; x-- )
					newBmp[ ( y - 1 ) * w + x ] = this->bmp[ ( y - 1 ) * this->width + x ];
		}
		
		// Finish work, copy buffers and free unused memory
		this->destruct();
		this->bmp = newBmp;
	}
	
	// adjust Size and reset clipping
	this->width = w;
	this->resetClippingRect();
}

void _bitmap::setHeight( _length h )
{
	_codeAnalyzer analyzer {"_bitmap::setHeight"};
	
	// limit
	h = max( h , _length(1) );
	
	if( this->height == h )
		return;
	
	// Only copy data if the underlying data is autoAllocated!
	if( this->wasAllocated && this->isValid() )
	{
		_pixelArray newBmp = new _pixel[ h * this->width ];
		_pixelArray tmpNew = newBmp;
		_pixelArray oldBmp = this->bmp;
		
		_u32 cnt = min( h , this->height ) * this->width;
		
		do
			*tmpNew++ = *oldBmp++;
		while( --cnt > 0 );
		
		// Release old Buffer and set new Buffer
		this->destruct();
		this->bmp = newBmp;
	}
	
	// adjust Size and reset clipping
	this->height = h;
	this->resetClippingRect();
}

void _bitmap::resize( _length w , _length h )
{
	_codeAnalyzer analyzer {"_bitmap::resize"};
	
	// limit
	h = max( h , _length(1) );
	w = max( w , _length(1) );
	
	//
	// Do the actual resizing
	//
	if( this->width == w )
		this->setHeight( h );
	
	if( this->isValid() )
	{		
		// Choose ptr to work on
		_pixelArray newBmp;
		
		if( this->wasAllocated )
			newBmp = new _pixel[ w * h ];
		else
			newBmp = this->bmp;
		
		// Resize
		if( w < this->width )
		{
			// Set y first to 1, because the first line of pixels will ever be the same
			for( _int y = 1; y < min( this->height , h ) ; y++ )
				for( _int x = 0; x < w ; x++ )
					newBmp[ y * w + x ] = this->bmp[ y * this->width + x ];
		}
		else if( w > this->width )
		{
			for( _int y = min( this->height , h ) ; y > 0 ; y-- )
				for( _int x = this->width - 1; x >= 0 ; x-- )
					newBmp[ ( y - 1 ) * w + x ] = this->bmp[ ( y - 1 ) * this->width + x ];
		}
		else if( this->wasAllocated )
		{
			_pixelArray tmpNew = newBmp;
			_pixelArray oldBmp = this->bmp;
			
			_u32 cnt = min( h , this->height ) * this->width;
			
			do
				*tmpNew++ = *oldBmp++;
			while( --cnt > 0 );
		}
		
		// Finish work, copy buffers, free the old (if needed)
		this->destruct();
		this->bmp = newBmp;
	}
	
	// adjust Size and reset clipping
	this->width = w;
	this->height = h;
	this->resetClippingRect(); 
}

void _bitmap::drawVerticalLine( _coord x , _coord y , _length length , _color color )
{
	_codeAnalyzer analyzer {"_bitmap::drawVerticalLine"};
	
	// Get end point of rect to draw
	_coord y2 = y + length - 1;
	
	// Attempt to clip
	if ( ! this->clipCoordinatesY( x , y , y2 ) ) return;
		
	// Calculate new height
	length = y2 - y + 1;
	
	if( length < 1 )
		return;
	
	// Pointer to BitMapBase
	_pixel* ptr = this->bmp + x + y * this->width;
	
	// Fill...
	do{
		*ptr = color;
		ptr += this->width;
	}while( --length > 0 );
}

void _bitmap::drawVerticalDottedLine( _coord x , _coord y , _length length , _color color )
{
	_codeAnalyzer analyzer {"_bitmap::drawVerticalDottedLine"};
	
	// Get end point of rect to draw
	_coord y2 = y + length - 1;
	
	// Attempt to clip
	if ( ! this->clipCoordinatesY( x , y , y2 ) ) return;
		
	// Calculate new height
	length = y2 - y + 1;
	
	// Pointer to BitMapBase
	_pixel* ptr = this->bmp + x + y * this->width;
	
	// Fill...
	do{
		*ptr = color;
		ptr += this->width << 1;
		length-=2;
	}while( length > 0 );
}

void _bitmap::drawHorizontalLine( _coord x , _coord y , _length length , _color color )
{
	_codeAnalyzer analyzer {"_bitmap::drawHorizontalLine"};
	
	// Get end point of rect to draw
	_coord x2 = x + length - 1;
	
	// Attempt to clip
	if ( ! this->clipCoordinatesX( x , y , x2 ) ) return;
		
	// Calculate new width
	length = x2 - x + 1;
	
	// Draw the line
	memset16( this->bmp + x + y * this->width , color , length );
}

void _bitmap::drawHorizontalDottedLine( _coord x , _coord y , _length length , _color color )
{
	_codeAnalyzer analyzer {"_bitmap::drawHorizontalDottedLine"};
	
	// Get end point of rect to draw
	_coord x2 = x + length - 1;
	
	// Attempt to clip
	if ( ! this->clipCoordinatesX( x , y , x2 ) ) return;
		
	// Calculate new width
	length = x2 - x + 1;
	
	// Pointer to BitMapBase
	_pixel* ptr = this->bmp + x + y * this->width;
	
	// Fill...
	do
	{
		*ptr++ = color;
		ptr++;
		length-=2;
	}while( length > 0 );
	
}

void _bitmap::drawDottedRect( _coord x , _coord y , _length w , _length h , _color color )
{
	_codeAnalyzer analyzer {"_bitmap::drawDottedRect"};
	
	if( !w || !h )
		return;
	
	if( w == 1 )
	{
		if( h == 1 )
			drawPixel( x , y , color );
		else
			this->drawVerticalDottedLine( x , y , h , color );
	}
	else if( h == 1 )
		this->drawHorizontalDottedLine( x , y , w , color );
	else
	{
		// Left and top part
		this->drawVerticalDottedLine( x , y , h , color );
		this->drawHorizontalDottedLine( x + 2 , y , w - 2 , color );
		
		_coord x2 = x + w - 1;
		_coord y2 = y + h - 1;
		
		// Right part
		if( w&1 ) // Not Divideable by 2
			this->drawVerticalDottedLine( x2 , y + 2 , h - 2 , color );
		else
			this->drawVerticalDottedLine( x2 , y + 1 , h - 1 , color );
		
		// Bottom Part
		if( h&1 ) // Not Divideable by 2
			this->drawHorizontalDottedLine( x + 2 , y2 , w - 3 , color );
		else
			this->drawHorizontalDottedLine( x + 1 , y2 , w - 2 , color );
	}
}

void _bitmap::drawRect( _coord x , _coord y , _length w , _length h , _color color )
{
	_codeAnalyzer analyzer {"_bitmap::drawRect"};
	
	if( !w || !h )
		return;
	
	if( w == 1 )
	{
		if( h == 1 )
			drawPixel( x , y , color );
		else
			drawVerticalLine( x , y , h , color );
	}
	else if( h == 1 )
		drawHorizontalLine( x , y , w , color );
	else
	{
		this->drawVerticalLine( x , y , --h , color );
		this->drawVerticalLine( x + --w  , y + 1 , h , color );
		this->drawHorizontalLine( x + 1, y , w , color );
		this->drawHorizontalLine( x , y + h  , w , color );
	}
}

void _bitmap::drawFilledRect( _coord x , _coord y , _length w , _length h , _color color )
{
	_codeAnalyzer analyzer {"_bitmap::drawFilledRect"};
	
	// Get end point of rect to draw
	_coord x2 = x + w - 1;
	_coord y2 = y + h - 1;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x , y , x2 , y2 ) ) return;
		
	// Calculate new width/height
	w = x2 - x + 1;
	h = y2 - y + 1;
	
	_pixelArray to = this->bmp + y * this->width + x;
	
	if( w == this->width )
		memset16( to , color , w * h ); // optimize Algorithm for drawing on whole bitmap
	else
	{
		// Add 7 to the height since we are dividing by 8
		h+= 7;
		
		int n,rem;
		swiDivMod( h , 8 , &n , &rem );
		
		switch( rem ) {
			case 7: do{ memset16( to , color , w );to += this->width;
			case 6:		memset16( to , color , w );to += this->width;
			case 5:		memset16( to , color , w );to += this->width;
			case 4:		memset16( to , color , w );to += this->width;
			case 3:		memset16( to , color , w );to += this->width;
			case 2:		memset16( to , color , w );to += this->width;
			case 1:		memset16( to , color , w );to += this->width;
			case 0:		memset16( to , color , w );to += this->width;
				} while( --n > 0 );
		}
	}
}

#define VERTICAL_GRADIENT_FILLER_RAND( rand1 , diff1 , diff2 , initialSeed ) \
	{\
		_u16 val = initialSeed;\
		\
		_u16 h1 = min( h , (_length)(diff2) );\
		h -= h1;\
		do\
		{\
			j = w;\
			do\
			{\
				if( ( val += (rand1) ) > 97 )\
					val -= 97;\
				pos[j-1] = *(temp + ( val & (diff2) ));\
			}while( --j > 0 );\
			pos += this->width;\
			temp++;	\
		}while( --h1 > 0 );\
		\
		h1 = min( h , _length( (diff2) + 1 ) );\
		h -= h1;\
		\
		if( h )\
		{\
			\
			do\
			{\
				j = w;\
				do\
				{\
					if( ( val += (rand1) ) > 97 )\
						val -= 97;\
					pos[j-1] = *( temp + ( val & (diff1) ) - (diff2) );\
				}while( --j > 0 );\
				pos += this->width;\
				temp++;\
			}while( --h > 0 );\
			\
			do\
			{\
				j = w;\
				do\
				{\
					if( ( val += (rand1) ) > 97 )\
						val -= 97;\
					pos[j-1] = *( temp + ( val & (diff2) ) - (diff2) );\
				}while( --j > 0 );\
				pos += this->width;\
				temp++;	\
			}while( --h1 > 0 );\
		}\
	}

void _bitmap::drawVerticalGradient( _coord x , _coord y , _length w , _length h , _color fromColor , _color toColor )
{
	_codeAnalyzer analyzer {"_bitmap::drawVerticalGradient"};
	
	if( fromColor == toColor )
	{
		this->drawFilledRect( x , y , w , h , fromColor );
		return;
	}
	
	_length gradHeight = h;
	_coord gradY = y;
	
	// Get end point of rect to draw
	_coord x2 = x + w - 1;
	_coord y2 = y + h - 1;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x , y , x2 , y2 ) ) return;
	
	// Calculate new width/height
	w = x2 - x + 1;
	h = y2 - y + 1;
	int dY = y - gradY;
	
	//! Gradient Computations
		
		_pixelArray gradTable = new _pixel[h];
		_pixelArray temp = gradTable;
		_pixelArray end = gradTable + h;
		
		// Difference between color values
		int dR = toColor.getR() - fromColor.getR();
		int dG = toColor.getG() - fromColor.getG();
		int dB = toColor.getB() - fromColor.getB();
		
		// Scale them to 2048
		dR <<= 11; dG <<= 11; dB <<= 11;
		
		int trigR = div32( dR , gradHeight );
		int trigG = div32( dG , gradHeight );
		int trigB = div32( dB , gradHeight );
		
		// Defines
		int curR = fromColor.getR() << 11;
		int curG = fromColor.getG() << 11;
		int curB = fromColor.getB() << 11;
		
		if( dY > 0 )
			curR += trigR*dY; curG += trigG*dY; curB += trigB*dY;
		
		// Fill the table
		for( ; temp != end ; curR += trigR, curG += trigG, curB += trigB )
			//*temp++ = _color::fromRGB( curR >> 11 , curG >> 11 , curB >> 11 );
			*temp++ = ( ( curR >> 11 ) & ( 31 << 0  ) ) // Adjust and bitwise AND width 31
					| ( ( curG >> 6  ) & ( 31 << 5  ) )
					| ( ( curB >> 1  ) & ( 31 << 10 ) )
					| ( ( 1 << 15 ) );
			// Faster because of less bitshifts
		
	// Number of pixels above each other having the same color
	_u32 difference = div32( 1 << 12 , abs( trigR ) + abs( trigG ) + abs( trigB ) );
	
	// Adjust end and reset temp variable
	temp = gradTable;
	end--;
	
	// Compute part to add at the end of each line
	_pixelArray pos = this->bmp + y * this->width + x;
	
	_u32 j = w;
	
	if( difference >= 8 )
		VERTICAL_GRADIENT_FILLER_RAND( 23 , 15 , 8 , x )
	else if( difference >= 4 )
		VERTICAL_GRADIENT_FILLER_RAND( 11 , 7 , 4 , x )
	else if( difference >= 2 )
		VERTICAL_GRADIENT_FILLER_RAND( 6 , 3 , 2 , x )
	else
	{
		_u16 val;
		
		do
		{
			j = w;
			val = *temp++;
			
			do
			{
				pos[j-1] = val;
			}while( --j > 0 );
			
			pos += this->width;
		}while( --h > 0 );
	}
	
	delete[] gradTable;
}

#undef VERTICAL_GRADIENT_FILLER_RAND

template<int rand1, int diff1, int diff2>
void horizontalGradientFillerHelper(
	_pixelArray		colorData
	, _length		gradientWidth
	, _length		gradientHeight
	, _pixelArray	drawDestination
	, _length		bitmapWidth
	, _int			initialSeed
)
{
	_u16 val = initialSeed;
	_pixelArray colorDataEnd = colorData + gradientWidth - 1;
	do
	{
		_length curX = gradientWidth;
		_pixelArray colorDataCur = colorDataEnd;
		do
		{
			val += (rand1);
			if( val > 97 )
				val -= 97;
			drawDestination[curX-1] = * min( max( colorData , (colorDataCur--) + ( val & (diff1) ) - (diff2) ) , colorDataEnd );
		}while( --curX );
		drawDestination += bitmapWidth;
	}while( --gradientHeight );
}

void _bitmap::drawHorizontalGradient( _coord x , _coord y , _length w , _length h , _color fromColor , _color toColor )
{
	_codeAnalyzer analyzer {"_bitmap::drawHorizontalGradient"};
	
	if( fromColor == toColor ){
		this->drawFilledRect( x , y , w , h , fromColor );
		return;
	}
	
	_length gradWidth = w;
	_coord origX = x;
	
	// Get end point of rect to draw
	_coord x2 = x + w - 1;
	_coord y2 = y + h - 1;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x , y , x2 , y2 ) ) return;
	
	// Calculate new width/height
	w = x2 - x + 1;
	h = y2 - y + 1;
	int dX = x - origX;
	
	_u32 numConsecutivePixels = 0; // Number of pixels above each other having the same color
	_uniquePtr<_pixel[]> gradientTable = computeGradient( fromColor , toColor , gradWidth , dX , &numConsecutivePixels );
	
	// Draw Set Bitmap-starting-ptr
	_pixelArray pos = this->bmp + y * this->width + x;
	
	if( numConsecutivePixels >= 8 )
		horizontalGradientFillerHelper< 23 , 15 , 7 >( gradientTable , w , h , pos , this->width , y );
	else if( numConsecutivePixels >= 4 )
		horizontalGradientFillerHelper< 11 , 7 , 3 >( gradientTable , w , h , pos , this->width , y );
	else if( numConsecutivePixels >= 2 )
		horizontalGradientFillerHelper< 6 , 3 , 1 >( gradientTable , w , h , pos , this->width , y );
	else
	{
		// Set end
		_pixelArray gradientTableEnd = _pixelArray( gradientTable ) + gradWidth - 1;
		
		// Loop Unwinding
		do
		{
			_u32 j = w;
			_pixelArray gradientTableCur = gradientTableEnd;
			
			do
			{
				pos[j-1] = *gradientTableCur--;
			}while( --j > 0 );
			
			pos += this->width;
		}while( --h > 0 );
	}
}

#undef HORIZONTAL_GRADIENT_FILLER_RAND

void _bitmap::drawCircle( _coord xc, _coord yc, _length radius, _color color )
{
	_codeAnalyzer analyzer {"_bitmap::drawCircle"};
	
	_s16 f = 1 - radius;
	_s16 ddF_x = 0;
	_s16 ddF_y = -2 * radius;
	_s16 x = 0;
	_s16 y = radius;

	this->drawPixel(xc, yc + radius, color);
	this->drawPixel(xc, yc - radius, color);
	this->drawPixel(xc + radius, yc, color);
	this->drawPixel(xc - radius, yc, color);

	while(x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;    
		this->drawPixel(xc + x, yc + y, color);
		this->drawPixel(xc - x, yc + y, color);
		this->drawPixel(xc + x, yc - y, color);
		this->drawPixel(xc - x, yc - y, color);
		this->drawPixel(xc + y, yc + x, color);
		this->drawPixel(xc - y, yc + x, color);
		this->drawPixel(xc + y, yc - x, color);
		this->drawPixel(xc - y, yc - x, color);
	}
}

void _bitmap::drawFilledCircle( _coord xc, _coord yc, _length radius, _color color )
{
	_codeAnalyzer analyzer {"_bitmap::drawFilledCircle"};
	
	_s16 f = 1 - radius;
	_s16 ddF_x = 0;
	_s16 ddF_y = -2 * radius;
	_s16 x = 0;
	_s16 y = radius;

	// Draw central line
	this->drawHorizontalLine(xc - radius, yc, (radius << 1) + 1, color);

	while(x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;
		
		this->drawHorizontalLine(xc - x, yc + y, (x << 1) + 1, color);
		this->drawHorizontalLine(xc - x, yc - y, (x << 1) + 1, color);
		
		this->drawHorizontalLine(xc - y, yc + x, (y << 1) + 1, color);
		this->drawHorizontalLine(xc - y, yc - x, (y << 1) + 1, color);
	}
}

void _bitmap::drawFilledEllipse( _coord xc, _coord yc, _length a, _length b, _color color)
{
	_codeAnalyzer analyzer {"_bitmap::drawFilledEllipse"};
	
	/* e(x,y) = b^2*x^2 + a^2*y^2 - a^2*b^2 */
	int x = 0, y = b;
	unsigned int width = 1;
	long a2 = (long)a*a, b2 = (long)b*b;
	long crit1 = -(a2/4 + a%2 + b2);
	long crit2 = -(b2/4 + b%2 + a2);
	long crit3 = -(b2/4 + b%2);
	long t = -a2*y; /* e(x+1/2,y-1/2) - (a^2+b^2)/4 */
	long dxt = 2*b2*x, dyt = -2*a2*y;
	long d2xt = 2*b2, d2yt = 2*a2;

	while ( y >= 0 && x <= _coord( a ) ) {
		if (t + b2*x <= crit1 ||     /* e(x+1,y-1/2) <= 0 */
			t + a2*y <= crit3) {     /* e(x+1/2,y) <= 0 */
			x++, dxt += d2xt, t += dxt; // incx()
			width += 2;
		}
		else if (t - a2*y > crit2) { /* e(x+1/2,y-1) > 0 */
			this->drawHorizontalLine(xc-x, yc-y, width , color );
			if (y!=0)
				this->drawHorizontalLine(xc-x, yc+y, width , color );
			y--, dyt += d2yt, t += dyt; // incy()
		}
		else {
			this->drawHorizontalLine(xc-x, yc-y, width , color );
			if (y!=0)
				this->drawHorizontalLine(xc-x, yc+y, width , color );
			x++, dxt += d2xt, t += dxt; // incx()
			y--, dyt += d2yt, t += dyt; // incy()
			width += 2;
		}
	}
	if (b == 0)
		this->drawHorizontalLine(xc-a, yc, 2*a+1 , color );
}

void _bitmap::drawEllipse( _coord xc, _coord yc, _length a, _length b, _color color)
{
	_codeAnalyzer analyzer {"_bitmap::drawEllipse"};
	
	/* e(x,y) = b^2*x^2 + a^2*y^2 - a^2*b^2 */
	int x = 0, y = b;
	unsigned int width = 1;
	long a2 = (long)a*a, b2 = (long)b*b;
	long crit1 = -(a2/4 + a%2 + b2);
	long crit2 = -(b2/4 + b%2 + a2);
	long crit3 = -(b2/4 + b%2);
	long t = -a2*y; /* e(x+1/2,y-1/2) - (a^2+b^2)/4 */
	long dxt = 2*b2*x, dyt = -2*a2*y;
	long d2xt = 2*b2, d2yt = 2*a2;

	while (y >= 0 && x <= _coord( a ) ) {
		if (t + b2*x <= crit1 ||     /* e(x+1,y-1/2) <= 0 */
			t + a2*y <= crit3) {     /* e(x+1/2,y) <= 0 */
			x++, dxt += d2xt, t += dxt; // incx()
			width += 2;
		}
		else if (t - a2*y > crit2) { /* e(x+1/2,y-1) > 0 */
			this->drawPixel(xc-x, yc-y, color );
			this->drawPixel(xc-x + width , yc-y, color );
			if (y!=0){
				this->drawPixel(xc-x, yc+y, color );
				this->drawPixel(xc-x + width , yc+y, color );
			}
			y--, dyt += d2yt, t += dyt; // incy()
		}
		else {
			this->drawPixel(xc-x, yc-y, color );
			this->drawPixel(xc-x + width , yc-y, color );
			if (y!=0){
				this->drawPixel(xc-x, yc+y, color );
				this->drawPixel(xc-x + width , yc+y, color );
			}
			x++, dxt += d2xt, t += dxt; // incx()
			y--, dyt += d2yt, t += dyt; // incy()
			width += 2;
		}
	}
	if (b == 0)
		this->drawHorizontalLine(xc-a, yc, 2*a+1 , color );
		this->drawHorizontalLine(xc-a, yc, 2*a+1 , color );
}

void _bitmap::drawString( _coord x0 , _coord y0 , _fontHandle font , _literal str , _color color , _u8 fontSize )
{
	_codeAnalyzer analyzer {"_bitmap::drawString"};
	
	if( !str || !*str )
		return;
	
	// Check if font is valid
	if( !font || !font->isValid() )
		return;
	
	// Check if clipping Rect is valid
	if( !this->activeClippingRect.isValid() || !this->isValid() )
		return;
	
	// Fetch the destination where to draw To
	_pixelArray dest = & this->bmp[ y0 * this->width + x0 ];
	
	do
	{
		if( x0 > this->activeClippingRect.getX2() )
			break;
		
		_length width = font->drawCharacter( dest , this->width , x0 , y0 , *str , color , this->activeClippingRect , fontSize );
		if( width )
			width += font->getLetterSpace();
		x0 += width;
		dest += width;
		
	}while( *++str );
}

void _bitmap::copy( _coord x , _coord y , _constBitmap& data )
{
	_codeAnalyzer analyzer {"_bitmap::copy"};
	
	if( !data.isValid() )
		return;
	
	_length dataWidth = data.getWidth();
	_coord x2 = x + dataWidth - 1;
	_coord y2 = y + data.getHeight() - 1;
	_coord origX = x;
	_coord origY = y;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x , y , x2 , y2 ) ) return;
	
	_length h = y2 - y + 1;
	_length w = x2 - x + 1;
	
	if( h <= 0 || w <= 0 )
		return;
	
	_pixelArray copyData = data.getBitmap( x - origX , y - origY );
	_pixelArray myData = this->getBitmap( x , y );
	
	do
	{
		memcpy16( myData , copyData , w );
		copyData += dataWidth;
		myData += this->width;
		
	}while( --h > 0 );
}

__attribute__((hot)) void _bitmap::copyTransparent( _coord x , _coord y , _constBitmap& data )
{
	_codeAnalyzer analyzer {"_bitmap::copyTransparent"};
	
	if( !data.isValid() )
		return;
	
	_coord x2 = x + data.getWidth() - 1;
	_coord y2 = y + data.getHeight() - 1;
	_coord origX = x;
	_coord origY = y;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x , y , x2 , y2 ) ) return;
	
	_pixelArray copyData = data.getBitmap( x - origX , y - origY );
	_pixelArray myData = this->getBitmap( x , y );
	
	_length h = y2 - y + 1;
	_length w = x2 - x + 1;
	
	if( h <= 0 || w <= 0 )
		return;
	
	// Compute Difference between the bitmap-Width and the actual copy-Width
	int dataDiff = data.getWidth() - w;
	int myDiff = this->width - w;
	
	#define COPY \
		{ \
		_pixel val = *copyData++; \
		if( val >> 15 ) \
			*myData = val; \
		myData++; \
		}
	
	// Add 15 to the width since we are dividing by 16
	w+= 15;
	
	int n, rem;
	swiDivMod( w , 16 , &n , &rem );
	
	//! Duff, B*tch!
	while( h-- )
	{
		int tN = n;
		switch( rem ) {
			case 15: do{COPY;
			case 14:	COPY;
			case 13:	COPY;
			case 12:	COPY;
			case 11:	COPY;
			case 10:	COPY;
			case 9:		COPY;
			case 8:		COPY;
			case 7:		COPY;
			case 6:		COPY;
			case 5:		COPY;
			case 4:		COPY;
			case 3:		COPY;
			case 2:		COPY;
			case 1:		COPY;
			case 0:		COPY;
				} while(--tN > 0);
		}
		// Advance both pointers so that they are at the next line
		copyData += dataDiff;
		myData += myDiff;
	}
	
	#undef COPY
}

void _bitmap::copyHorizontalStretch( _coord x , _coord y , _length w , _constBitmap& data )
{
	_codeAnalyzer analyzer {"_bitmap::copyHorizontalStretch"};
	
	_coord x2 = x + w - 1;
	_coord y2 = y + data.getHeight() - 1;
	_coord origY = y;
	
	if( !data.isValid() )
		return;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x , y , x2 , y2 ) ) return;
	
	_pixelArray copyData = data.getBitmap();
	if( !copyData )
		return;
	_pixelArray myData	= &this->bmp[ y * this->width + x ];
	
	_length height = y2 - y + 1;
	_length width = x2 - x + 1;
	
	for( _int i = 0; i < height; i++ )
	{
		memset16( myData , copyData[y - origY + i] , width );
		myData += this->width;
	}
}

void _bitmap::copyVerticalStretch( _coord x , _coord y , _length h , _constBitmap& data )
{
	_codeAnalyzer analyzer {"_bitmap::copyVerticalStretch"};
	
	_coord x2 = x + data.getWidth() - 1;
	_coord y2 = y + h - 1;
	_coord origX = x;
	
	if( !data.isValid() )
		return;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x , y , x2 , y2 ) ) return;
	
	// Get Data
	_pixelArray copyData = &data.operator[]( + x - origX );
	if( !copyData )
		return;
	_pixelArray myData	= &this->bmp[ y * this->width + x ];
	
	_length height = y2 - y;
	_length width = x2 - x + 1;
	
	// Draw one line!
	memcpy16( myData , copyData , width );
	
	_pixelArray destination = myData;
	
	// Copy that line!
	do{
		destination += this->width;
		memcpy16( destination , myData , width );
	}while( --height > 0 );
}

void _bitmap::move( _coord sourceX , _coord sourceY , _coord destX , _coord destY , _length width , _length height )
{
	_codeAnalyzer analyzer {"_bitmap::move"};
	
	// Do nothing if no copying involved
	if ( ( sourceX == destX ) && ( sourceY == destY ) ) return;

	// Get end point of source
	_coord x2 = sourceX + width - 1;
	_coord y2 = sourceY + height - 1;

	// Attempt to clip
	if ( ! this->clipCoordinates( sourceX , sourceY , x2, y2 ) ) return;

	// Convert width and height back so that we can calculate the dimesions of the dest
	width = x2 + 1 - sourceX;
	height = y2 + 1 - sourceY;

	// Get end point of dest
	x2 = destX + width - 1;
	y2 = destY + height - 1;
	
	_s16 dX = destX;

	// Attempt to clip
	if ( ! this->clipCoordinates( destX , destY , x2 , y2 ) ) return;

	// Convert width and height back again for use in the rest of the function
	width = x2 + 1 - destX;
	height = y2 + 1 - destY;
	
	dX -= destX;
	
	// If there is no vertical movement and the source and destination overlap,
	// we need to use an offscreen buffer to copy
	if ( sourceY == destY )
	{
		if ( ( destX >= sourceX ) && ( destX <= _coord( width + sourceX ) ) )
		{
			_pixelArray buffer = new _pixel[width+dX];
			
			for ( _int y = 0; y != height; ++y )
			{
				// Copy row to buffer
				memcpy16( buffer , this->bmp + sourceX - dX + this->width * ( sourceY + y ) , width );
				
				// Copy row back to screen
				memcpy16( this->bmp + destX + this->width * ( sourceY + y ) , buffer , width );
			}
			
			delete[] buffer;
			
			return;
		}
	}
	
	// Vertical movement or non overlap means we do not need to use an intermediate buffer
	// Copy from top to bottom if moving up; from bottom to top if moving down.
	// Ensures that rows to be copied are not overwritten
	if (sourceY > destY)
	{
		_pixelArray src = this->bmp + sourceX - dX + this->width * sourceY;
		_pixelArray dst = this->bmp + destX + this->width * destY;
		
		// Copy up
		for ( _int i = 0 ; i < height ; ++i , src += this->width , dst += this->width )
			memcpy16( dst , src , width );
	}
	else
	{
		_pixelArray src = this->bmp + sourceX - dX + this->width * ( sourceY + height - 1 );
		_pixelArray dst = this->bmp + destX + this->width * ( destY + height - 1 );
		
		// Copy down
		for ( _u32 i = height ; i > 0 ; --i , src -= this->width , dst -= this->width )
			memcpy16( dst , src , width );
	}
}


optimized bool _bitmap::clipCoordinates( _coord &left , _coord &top , _coord &right , _coord &bottom ) const 
{
	_codeAnalyzer analyzer {"_bitmap::clipCoordinates"};
	
	if( !this->activeClippingRect.isValid() || !this->isValid() )
		return false;
	
	_coord x = activeClippingRect.x;
	_coord x2 = activeClippingRect.getX2();
	
	// Ensure values don't exceed clipping rectangle
	left 	= max( left , x );
	right 	= min( right , x2 );
	
	if( right < left )
		return false;
	
	_coord y = activeClippingRect.y;
	_coord y2 = activeClippingRect.getY2();
	
	top 	= max( top , y );
	bottom 	= min( bottom , y2 );
	
	// Return false if no box to draw
	// Return true as box can be drawn
	return bottom >= top;
}

optimized bool _bitmap::clipCoordinatesX( _coord &left , _coord &top , _coord &right ) const 
{
	_codeAnalyzer analyzer {"_bitmap::clipCoordinatesX"};
	
	if( !this->activeClippingRect.isValid() )
		return false;
	
	_coord x = activeClippingRect.x;
	
	// Ensure values don't exceed clipping rectangle
	left 	= max( left , x );
	right 	= min( right , _coord( x + _coord( activeClippingRect.width ) - 1 ) );
	
	return right >= left && top >= activeClippingRect.y && top <= activeClippingRect.getY2();
}

optimized bool _bitmap::clipCoordinatesY( _coord &left , _coord &top , _coord &bottom ) const 
{
	_codeAnalyzer analyzer {"_bitmap::clipCoordinatesY"};
	
	if( !this->activeClippingRect.isValid() )
		return false;
	
	_coord y = activeClippingRect.y;
	
	// Ensure values don't exceed clipping rectangle
	top 	= max( top , y );
	bottom 	= min( bottom , _coord( y + _coord( activeClippingRect.height ) - 1 ) );
	
	return bottom >= top && left >= activeClippingRect.x && left <= activeClippingRect.getX2();
}

// Internal routine
void _bitmap::drawClippedLine( _coord x1 , _coord y1 , _coord x2 , _coord y2 , _color color )
{
	_codeAnalyzer analyzer {"_bitmap::drawClippedLine"};
	
	// Get the Bitmap pointer
    _pixelArray offset = this->bmp + y1 * this->width + x1;
	
	_int errorTerm = 0;
    _int i;
	
    _int xDiff = x2 - x1;
    _int yDiff = y2 - y1;
	
	_int xStep;
	_int yStep;
	
	// Compute xStep
	if( xDiff < 0 )
	{
		xStep = -1;
		xDiff--;
		xDiff = -xDiff; // Increase and make positive
	}
	else
	{
		xDiff++;
		xStep = 1;
	}
	
	// Compute yStep
	if( yDiff < 0 )
	{
		yStep = -this->width;
		yDiff--;
		yDiff = -yDiff; // Increase and make positive
	}
	else
	{
		yDiff++;
		yStep = this->width;
	}
 
    //case for changes more in X than in Y
    if ( xDiff > yDiff )
    {
		i = xDiff;
		
		errorTerm = yDiff >> 1;
		
		do
		{
			*offset = color;
			
			errorTerm += yDiff;
			
			if (errorTerm >= xDiff)
			{  
				errorTerm -= xDiff;
				offset += yStep + xStep;
			}
			else
				offset += xStep;
		}while( --i > 0 );
		
    }//end if xdiff > ydiff
    //case for changes more in Y than in X
    else if( xDiff < yDiff )
    {
		i = yDiff;
		
		errorTerm = xDiff >> 1;
		
		// We already know that yDiff cannot be 0
		do
		{
			*offset = color;
			
			errorTerm += xDiff;
			
			if (errorTerm >= yDiff)
			{
				errorTerm -= yDiff;
				offset += xStep + yStep;
			}
			else
				offset += yStep;
			
		}while( --i > 0 );
    }
	else // For straight diagonal Line
	{
		i = yDiff;
		
		do
		{
			*offset = color;
			offset += xStep + yStep;
		}while( --i > 0 );
	}
}

_u8 _bitmap::getClipLineOutCode( _coord x , _coord y , _coord xMin , _coord yMin , _coord xMax , _coord yMax )
{
	_u8 code = 0;
	
	if( y > yMax )
		code |= 1;
	else if( y < yMin )
		code |= 2;
	
	if( x > xMax )
		code |= 4;
	else if(x < xMin)
		code |= 8;
	
	return code;
}

void _bitmap::drawLine( _coord x1 , _coord y1 , _coord x2 , _coord y2 , _color color )
{
	_codeAnalyzer analyzer {"_bitmap::drawLine"};
	
	// Choose shortcut for horizontal or vertical lines
	if ( x1 == x2 )
	{
		drawVerticalLine( x1 , min( y1 , y2 ) , abs( y2 - y1 ) + 1 , color );
		return;
	}
	else if ( y1 == y2 )
	{
		drawHorizontalLine( min( x1 , x2 ) , y1 , abs( x2 - x1 ) + 1 , color );
		return;
	}

	// Extract data from cliprect
	_coord minX = activeClippingRect.x;
	_coord minY = activeClippingRect.y;
	_coord maxX = activeClippingRect.getX2();
	_coord maxY = activeClippingRect.getY2();
		
	// Get outcodes for each point
	_u8 code1 = getClipLineOutCode(x1, y1, minX, minY, maxX, maxY);
	_u8 code2 = getClipLineOutCode(x2, y2, minX, minY, maxX, maxY);
	
	// Clip
	while( true )
	{
		// Check for trivial cases
		if ( !( code1 | code2 ) )
		{
			// Line entirely within visible region
			// Draw the line
			this->drawClippedLine(x1, y1, x2, y2, color);
			break;
		}
		else if( code1 & code2 )
			// Both end points fall within the same off-screen region
			break;
		else {
			
			// No trivial accept
			_coord x = 0;
			_coord y = 0;
			_int t = 0;
			
			// Choose one of the end points to manipulate (only choose
			// the end point that is still off screen)
			_u8 codeout = code1 ? code1 : code2;
			
			// Check each region
			if( codeout & 1 )
			{
				// Check top value
				t = ((maxY - y1) << 8) / (y2 - y1);
				x = x1 + ((t * (x2 - x1)) >> 8);
				y = maxY;
			}
			else if( codeout & 2 )
			{
				// Check bottom value
				t = ((minY - y1) << 8) / (y2 - y1);
				x = x1 + ((t * (x2 - x1)) >> 8);
				y = minY;
			}
			else if( codeout & 4 )
			{
				// Check right value
				t = ((maxX - x1) << 8) / (x2 - x1);
				y = y1 + ((t * (y2 - y1)) >> 8);
				x = maxX;
			}
			else if( codeout & 8 )
			{
				// Check left value
				t = ((minX - x1) << 8) / (x2 - x1);
				y = y1 + ((t * (y2 - y1)) >> 8);
				x = minX;
			}
			
			// Check to see which endpoint we clipped
			if( codeout == code1 )
			{
				// First endpoint clipped; update first point
				x1 = x;
				y1 = y;
				
				// Clip again
				code1 = getClipLineOutCode(x1, y1, minX, minY, maxX, maxY);
			}
			else
			{
				// Second endpoint clipped; update second point
				x2 = x;
				y2 = y;
				
				// Clip again
				code2 = getClipLineOutCode(x2, y2, minX, minY, maxX, maxY);
			}
		}
	}
}

_bitmap& _bitmap::operator=( _constBitmap& bmp )
{
	_codeAnalyzer analyzer {"_bitmap::operator=( _constBitmap& bmp )"};
	
	// Copy..
	if( this == &bmp )
		return *this;
	
	this->destruct();
	this->width = bmp.width;
	this->height = bmp.height;
	this->wasAllocated = bmp.wasAllocated;
	
	if( wasAllocated )
	{
		this->bmp = new _pixel[this->width*this->height];
		memcpy16( this->bmp , bmp.bmp , this->width * this->height );
	}
	else
		this->bmp = bmp.bmp;
	
	this->resetClippingRect();
	
	return *this;
}

_bitmap& _bitmap::operator=( _bitmap&& bmp )
{
	_codeAnalyzer analyzer {"_bitmap::operator=( _bitmap&& bmp )"};
	
	// Just in case...
	if( this == &bmp )
		return *this;
	
	this->destruct();
	this->width = bmp.width;
	this->height = bmp.height;
	this->wasAllocated = bmp.wasAllocated;
	
	// Move Memory
	this->bmp = bmp.bmp;
	
	// Reset
	bmp.wasAllocated = false;
	bmp.bmp = nullptr;
	
	this->resetClippingRect();
	
	return *this;
}