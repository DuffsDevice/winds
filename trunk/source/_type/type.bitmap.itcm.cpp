#include "_type/type.bitmap.h"
#include "_type/type.textphrases.h"
#include "func.memory.h"

#include <nds/arm9/math.h>

void _bitmap::setWidth( _length w )
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a = _codeAnalyzer( "_bitmap::setWidth" );
	#endif
	
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
			memSet( newBmp , 0 , w * this->height );
		}
		else
			newBmp = this->bmp;
			
		if( w < this->width )
		{
			for( _u32 y = 0; y != this->height ; y++ )
				for( _u32 x = 0; x != w ; x++ )
					newBmp[ y * w + x ] = this->bmp[ y * this->width + x ];
		}
		else
		{
			for( _u32 y = this->height ; y != 0 ; y-- )
				for( _u32 x = this->width ; x != 0 ; x-- )
					newBmp[ ( y - 1 ) * w + x - 1 ] = this->bmp[ ( y - 1 ) * this->width + x - 1 ];
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
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::setHeight" );
	#endif
	
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
		while( --cnt );
		
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
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a = _codeAnalyzer( "_bitmap::resize" );
	#endif
	
	// limit
	h = max( h , _length(1) );
	w = max( w , _length(1) );
	
	//
	// Do the actual resizing
	//
	
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
			for( _u32 y = 0; y != min( this->height , h ) ; y++ )
				for( _u32 x = 0; x != w ; x++ )
					newBmp[ y * w + x ] = this->bmp[ y * this->width + x ];
		}
		else if( w > this->width )
		{
			for( _u32 y = min( this->height , h ) ; y != 0 ; y-- )
				for( _u32 x = this->width ; x != 0 ; x-- )
					newBmp[ ( y - 1 ) * w + x - 1 ] = this->bmp[ ( y - 1 ) * this->width + x - 1 ];
		}
		else
		{
			_pixelArray tmpNew = newBmp;
			_pixelArray oldBmp = this->bmp;
			
			_u32 cnt = min( h , this->height ) * this->width;
			
			do
				*tmpNew++ = *oldBmp++;
			while( --cnt );
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

void _bitmap::drawVerticalLine( _coord x , _coord y , _length length , _pixel color )
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::drawVerticalLine" );
	#endif
	
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
	}while( --length );
}

void _bitmap::drawVerticalDottedLine( _coord x , _coord y , _length length , _pixel color )
{
	// Get end point of rect to draw
	_coord y2 = y + length - 1;
	
	// Attempt to clip
	if ( ! this->clipCoordinatesY( x , y , y2 ) ) return;
		
	// Calculate new height
	length = ( y2 - y + 1 ) >> 1;
	
	// Pointer to BitMapBase
	_pixel* ptr = this->bmp + x + y * this->width;
	
	// Fill...
	while( length-- )
	{
		*ptr = color;
		ptr += this->width << 1;
	}
}

void _bitmap::drawHorizontalLine( _coord x , _coord y , _length length , _pixel color )
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::drawHorizontalLine" );
	#endif
	
	// Get end point of rect to draw
	_coord x2 = x + length - 1;
	
	// Attempt to clip
	if ( ! this->clipCoordinatesX( x , y , x2 ) ) return;
		
	// Calculate new width
	length = x2 - x + 1;
	
	// Draw the line
	memSet( this->bmp + x + y * this->width , color , length );
}

void _bitmap::drawHorizontalDottedLine( _coord x , _coord y , _length length , _pixel color )
{
	// Get end point of rect to draw
	_coord x2 = x + length - 1;
	
	// Attempt to clip
	if ( ! this->clipCoordinatesX( x , y , x2 ) ) return;
		
	// Calculate new width
	length = ( x2 - x + 1 ) >> 1;
	
	// Pointer to BitMapBase
	_pixel* ptr = this->bmp + x + y * this->width;
	
	// Fill...
	while( length-- )
	{
		*ptr++ = color;
		ptr++;
	}
	
}
void _bitmap::drawRect( _coord x , _coord y , _length w , _length h , _pixel color )
{
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

void _bitmap::drawFilledRect( _coord x , _coord y , _length w , _length h , _pixel color )
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::drawFilledRect" );
	#endif
	
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
		memSet( to , color , w * h ); // optimize Algorithm for drawing on whole bitmap
	else
	{
		// Add 15 to the height since we are dividing by 8
		h+= 7;
		
		int n = h / 8;
		int rem = h % 8;
		
		int tN = n;
		switch( rem ) {
			case 7: do{ memSet( to , color , w );to += this->width;
			case 6:		memSet( to , color , w );to += this->width;
			case 5:		memSet( to , color , w );to += this->width;
			case 4:		memSet( to , color , w );to += this->width;
			case 3:		memSet( to , color , w );to += this->width;
			case 2:		memSet( to , color , w );to += this->width;
			case 1:		memSet( to , color , w );to += this->width;
			case 0:		memSet( to , color , w );to += this->width;
				} while(--tN > 0);
		}
	}
}

void _bitmap::replaceColor( _pixel color , _pixel replace )
{
	if( !this->height || !this->width )
		return;
	
	_u32 cnt = this->height * this->width;
	
	_pixelArray ptr = this->bmp;
	
	// Replace all values with passed color
	if( RGB_GETA( color ) )
	{
		do
		{
			if( *ptr == color )
				*ptr = replace;
		}while( --cnt );
	}
	else
	{
		do
		{
			if( !RGB_GETA( *ptr ) )
				*ptr = replace;
		}while( --cnt );
	}
}

#define VERTICAL_GRADIENT_FILLER_RAND( rand1 , diff1 , diff2 ) \
	{\
		_u16 val = 0;\
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
			}while( --j );\
			pos += this->width;\
			temp++;	\
		}while( --h1 );\
		\
		h1 = min( h , _length( (diff2) + 1 ) );\
		h -= h1;\
		\
		if( !h )\
			return;\
		\
		do\
		{\
			j = w;\
			do\
			{\
				if( ( val += (rand1) ) > 97 )\
					val -= 97;\
				pos[j-1] = *( temp + ( val & (diff1) ) - (diff2) );\
			}while( --j );\
			pos += this->width;\
			temp++;\
		}while( --h );\
		\
		do\
		{\
			j = w;\
			do\
			{\
				if( ( val += (rand1) ) > 97 )\
					val -= 97;\
				pos[j-1] = *( temp + ( val & (diff2) ) - (diff2) );\
			}while( --j );\
			pos += this->width;\
			temp++;	\
		}while( --h1 );\
	}

void _bitmap::drawVerticalGradient( _coord x , _coord y , _length w , _length h , _pixel fromColor , _pixel toColor )
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::drawVerticalGradient" );
	#endif
	
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
		int dR = RGB_GETR(toColor) - RGB_GETR(fromColor);
		int dG = RGB_GETG(toColor) - RGB_GETG(fromColor);
		int dB = RGB_GETB(toColor) - RGB_GETB(fromColor);
		
		// Scale them to 2048
		dR <<= 11; dG <<= 11; dB <<= 11;
		
		int trigR = div32( dR , gradHeight );
		int trigG = div32( dG , gradHeight );
		int trigB = div32( dB , gradHeight );
		
		// Defines
		int curR = RGB_GETR(fromColor) << 11;
		int curG = RGB_GETG(fromColor) << 11;
		int curB = RGB_GETB(fromColor) << 11;
		
		if( dY > 0 )
			curR += trigR*dY; curG += trigG*dY; curB += trigB*dY;
		
		// Fill the table
		for( ; temp != end ; curR += trigR, curG += trigG, curB += trigB )
			//*temp++ = RGB( curR >> 11 , curG >> 11 , curB >> 11 );
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
		VERTICAL_GRADIENT_FILLER_RAND( 23 , 15 , 8 )
	else if( difference >= 4 )
		VERTICAL_GRADIENT_FILLER_RAND( 11 , 7 , 4 )
	else if( difference >= 2 )
		VERTICAL_GRADIENT_FILLER_RAND( 6 , 3 , 2 )
	else
	{
		_u16 val;
		// Loop Unwinding
		do
		{
			j = w;
			val = *temp++;
			
			do
			{
				pos[j-1] = val;
			}while( --j );
			
			pos += this->width;
		}while( --h );
	}
	
	delete[] gradTable;
}

#undef VERTICAL_GRADIENT_FILLER_RAND

#define HORIZONTAL_GRADIENT_FILLER_RAND( rand1 , diff1 , diff2 ) \
{\
	_u16 val = 0;\
	do\
	{\
		temp = end;\
		j = w;\
		do\
		{\
			val += (rand1);\
			if( val > 97 )\
				val -= 97;\
			pos[j-1] = * min( max( gradTable , (temp--) + ( val & (diff1) ) - (diff2) ) , end );\
		}while( --j );\
		pos += this->width;\
	}while( --h );\
}

void _bitmap::drawHorizontalGradient( _coord x , _coord y , _length w , _length h , _pixel fromColor , _pixel toColor )
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::drawHorizontalGradient" );
	#endif
	
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
	
	//! Gradient Computations
		
		_pixelArray gradTable = new _pixel[w];
		_pixelArray start = gradTable;
		_pixelArray temp = start;
		_pixelArray end = gradTable + w;
		
		// Difference between color values
		int dR = RGB_GETR(toColor) - RGB_GETR(fromColor);
		int dG = RGB_GETG(toColor) - RGB_GETG(fromColor);
		int dB = RGB_GETB(toColor) - RGB_GETB(fromColor);
		
		// Scale them to 512
		dR <<= 11; dG <<= 11; dB <<= 11;
		
		int trigR = div32( dR , gradWidth );
		int trigG = div32( dG , gradWidth );
		int trigB = div32( dB , gradWidth );
		
		// Defines
		unsigned int curR = ( fromColor & 31 ) << 11; // Faster than RGB_GETR... ??
		unsigned int curG = RGB_GETG(fromColor) << 11;
		unsigned int curB = RGB_GETB(fromColor) << 11;
		
		if( dX > 0 )
		{
			curR += trigR * dX;
			curG += trigG * dX;
			curB += trigB * dX;
		}
		
		// Fill the table
		for( ; temp != end ; curR += trigR, curG += trigG, curB += trigB )
			//*temp++ = RGB( curR >> 11 , curG >> 11 , curB >> 11 );
			*temp++ = ( ( curR >> 11 ) & ( 31 << 0  ) ) // Adjust and bitwise and width 31
					| ( ( curG >> 6  ) & ( 31 << 5  ) )
					| ( ( curB >> 1  ) & ( 31 << 10 ) )
					| ( ( 1 << 15 ) );
			// Faster because of less bitshifts
		
	// Draw Set Bitmap-starting-ptr
	_pixelArray pos = this->bmp + y * this->width + x;
	
	// Number of pixels above each other having the same color
	_u32 difference = div32( 1 << 12 , abs( trigR ) + abs( trigG ) + abs( trigB ) );
	
	// Adjust end
	end--;
	
	_u32 j = w;
	
	if( difference >= 8 )
		HORIZONTAL_GRADIENT_FILLER_RAND( 23 , 15 , 7 )
	else if( difference >= 4 )
		HORIZONTAL_GRADIENT_FILLER_RAND( 11 , 7 , 3 )
	else if( difference >= 2 )
		HORIZONTAL_GRADIENT_FILLER_RAND( 6 , 3 , 1 )
	else
	{
		// Loop Unwinding
		do
		{
			j = w;
			temp = end;
			do
			{
				pos[j-1] = *temp--;
			}while( --j );
			
			pos += this->width;
		}while( --h );
	}
	
	delete[] gradTable;
}

#undef HORIZONTAL_GRADIENT_FILLER_RAND

void _bitmap::drawCircle( _coord xc, _coord yc, _length radius, _pixel color )
{			
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

void _bitmap::drawFilledCircle( _coord xc, _coord yc, _length radius, _pixel color )
{			
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

void _bitmap::drawFilledEllipse( _coord xc, _coord yc, _length a, _length b, _pixel color)
{				
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

void _bitmap::drawEllipse( _coord xc, _coord yc, _length a, _length b, _pixel color)
{				
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

void _bitmap::drawString( _coord x0 , _coord y0 , const _font* font , const _char* str , _pixel color , _u8 fontSize )
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::drawString" );
	#endif
	
	if( !str || !*str )
		return;
	
	// Check if font is valid
	if( !font || !font->isValid() )
		return;
	
	// Fetch the destination where to draw To
	_pixelArray dest = & this->bmp[ y0 * this->width + x0 ];
	
	do
	{
		if( x0 > this->activeClippingRect.getX2() )
			break;
		
		if( stringExtractor::processChar( str , fontSize , font , color ) )
		{
			_length width = font->drawCharacter( dest , this->width , x0 , y0 , *str , color , this->activeClippingRect , fontSize );
			if( width )
				width++;
			x0 += width;
			dest += width;
		}
		
	}while( *++str );
}

void _bitmap::copy( _coord x , _coord y , const _bitmap& data )
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::copy" );
	#endif
	
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
	_pixelArray myData= this->getBitmap( x , y );
	
	do
	{
		memCpy( myData , copyData , w );
		copyData += dataWidth;
		myData += this->width;
		
	}while( --h );
}

void _bitmap::copyTransparent( _coord x , _coord y , const _bitmap& data )
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::copyTransparent" );
	#endif
	
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
	
	int n = w / 16;
	int rem = w % 16;
	
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

void _bitmap::copyHorizontalStretch( _coord x , _coord y , _length w , const _bitmap& data )
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::copyHorizontalStretch" );
	#endif
	
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
	
	for( _u32 i = 0; i != height; i++ )
	{
		memSet( myData , copyData[y - origY + i] , width );
		myData += this->width;
	}
}

void _bitmap::copyVerticalStretch( _coord x , _coord y , _length h , const _bitmap& data )
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::copyVerticalStretch" );
	#endif
	
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
	memCpy( myData , copyData , width );
	
	_pixelArray destination = myData;
	
	// Copy that line!
	while( height-- )
	{
		destination += this->width;
		memCpy( destination , myData , width );
	}
}

void _bitmap::move( _coord sourceX , _coord sourceY , _coord destX , _coord destY , _length width , _length height )
{
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
			
			for ( _u32 y = 0; y != height; ++y )
			{
				// Copy row to buffer
				memCpy( buffer , this->bmp + sourceX + dX + this->width * ( sourceY + y ) , width + dX );
				
				// Copy row back to screen
				memCpy( this->bmp + destX + dX + this->width * ( destX + y ) , buffer , width + dX );
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
		for ( _u32 i = 0 ; i != height ; ++i , src += this->width , dst += this->width )
			memCpy( dst , src , width );
	}
	else
	{
		_pixelArray src = this->bmp + sourceX - dX + this->width * ( sourceY + height - 1 );
		_pixelArray dst = this->bmp + destX + this->width * ( destY + height - 1 );
		
		// Copy down
		for ( _u32 i = height ; i ; --i , src -= this->width , dst -= this->width )
			memCpy( dst , src , width );
	}
}


bool _bitmap::clipCoordinates( _coord &left , _coord &top , _coord &right , _coord &bottom ) const 
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::clipCoordinates" );
	#endif
	
	if( !this->activeClippingRect.isValid() )
		return false;
	
	int x = activeClippingRect.x;
	int x2 = activeClippingRect.getX2();
	
	// Ensure values don't exceed clipping rectangle
	left 	= max( (int)left , x );
	right 	= min( (int)right , x2 );
	
	if( right < left )
		return false;
	
	int y = activeClippingRect.y;
	int y2 = activeClippingRect.getY2();
	
	top 	= max( (int)top , y );
	bottom 	= min( (int)bottom , y2 );
	
	// Return false if no box to draw
	// Return true as box can be drawn
	return bottom >= top;
}

bool _bitmap::clipCoordinatesX( _coord &left , _coord &top , _coord &right ) const 
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::clipCoordinatesX" );
	#endif
	
	if( !this->activeClippingRect.isValid() )
		return false;
	
	int x = activeClippingRect.x;
	
	// Ensure values don't exceed clipping rectangle
	left 	= max( (int)left , x );
	right 	= min( (int)right , int( x + int( activeClippingRect.width ) - 1 ) );
	
	return right >= left && top >= activeClippingRect.y && top <= activeClippingRect.getY2();
}

bool _bitmap::clipCoordinatesY( _coord &left , _coord &top , _coord &bottom ) const 
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::clipCoordinatesX" );
	#endif
	
	if( !this->activeClippingRect.isValid() )
		return false;
	
	int y = activeClippingRect.y;
	
	// Ensure values don't exceed clipping rectangle
	top 	= max( (int)top , y );
	bottom 	= min( (int)bottom , int( y + int( activeClippingRect.height ) - 1 ) );
	
	return bottom >= top && left >= activeClippingRect.x && left <= activeClippingRect.getX2();
}

// Internal routine
void _bitmap::drawClippedLine( _coord x1 , _coord y1 , _coord x2 , _coord y2 , _pixel color )
{
	// Get the Bitmap pointer
    _pixelArray offset = this->bmp + y1 * this->width + x1;
	
	_s32 errorTerm = 0;
    _s32 i;
	
    _s32 xDiff = x2 - x1;
    _s32 yDiff = y2 - y1;
	
	_s32 xStep;
	_s32 yStep;
	
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
		}while( --i );
		
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
			
		}while( --i );
    }
	else // For straight diagonal Line
	{
		i = yDiff;
		
		do
		{
			*offset = color;
			offset += xStep + yStep;
		}while( --i );
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

void _bitmap::drawLine( _coord x1 , _coord y1 , _coord x2 , _coord y2 , _pixel color )
{
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
			_s32 t = 0;
			
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

_bitmap& _bitmap::operator=( const _bitmap& bmp )
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::copyOperator" );
	#endif
	
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
		memCpy( this->bmp , bmp.bmp , this->width * this->height );
	}
	else
		this->bmp = bmp.bmp;
	
	this->resetClippingRect();
	
	return *this;
}

_bitmap& _bitmap::operator=( _bitmap&& bmp )
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_bitmap::moveOperator" );
	#endif
	
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