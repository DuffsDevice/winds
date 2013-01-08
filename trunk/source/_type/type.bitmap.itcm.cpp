#include "_type/type.bitmap.h"
#include "func.memory.h"

#include <nds/timers.h>
#include <nds/arm9/math.h>

void _bitmap::setWidth( _length w )
{ 
	if( this->width == w )
		return;
	
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
	
	if( this->wasAllocated )
	{
		delete this->bmp;
		this->bmp = newBmp;
	}
	
	this->width = w;
	this->resetClippingRect(); 
}

void _bitmap::setHeight( _length h )
{
	_pixelArray newBmp;	
	
	if( this->wasAllocated )
	{
		newBmp = new _pixel[ h * this->width ];
		//memSet( newBmp , 0 , h * this->width );
	}
	else
		newBmp = this->bmp;
	
	memCpy( newBmp , this->bmp , min( this->height , h ) * this->width );
	
	if( this->wasAllocated )
	{
		delete this->bmp;
		this->bmp = newBmp;
	}
	
	this->height = h;
	this->resetClippingRect();
}

void _bitmap::resize( _length w , _length h )
{
	if( this->width == w && this->height == h )
		return;
	
	_pixelArray newBmp;	
	
	if( this->wasAllocated )
	{
		newBmp = new _pixel[ w * h ];
		//memSet( newBmp , 0 , w * this->height );
	}
	else
		newBmp = this->bmp;
	
	if( w < this->width )
	{
		for( _u32 y = 0; y != min( this->height , h ) ; y++ )
			for( _u32 x = 0; x != w ; x++ )
				newBmp[ y * w + x ] = this->bmp[ y * this->width + x ];
	}
	else
	{
		for( _u32 y = min( this->height , h ) ; y != 0 ; y-- )
			for( _u32 x = this->width ; x != 0 ; x-- )
				newBmp[ ( y - 1 ) * w + x - 1 ] = this->bmp[ ( y - 1 ) * this->width + x - 1 ];
	}
	
	if( this->wasAllocated )
	{
		delete this->bmp;
		this->bmp = newBmp;
	}
	
	this->width = w;
	this->height = h;
	this->resetClippingRect(); 
}

void _bitmap::drawPixel( _coord x , _coord y , _pixel color )
{
	if( activeClippingRect.contains( x , y ) )
		this->bmp[y * this->width + x] = color;
}

void _bitmap::drawVerticalLine( _coord x , _coord y , _length length , _pixel color )
{
	// Get end point of rect to draw
	_coord x2 = x;
	_coord y2 = y + length - 1;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
		
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
	_coord x2 = x;
	_coord y2 = y + length - 1;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
		
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
	// Get end point of rect to draw
	_coord x2 = x + length - 1;
	_coord y2 = y;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
		
	// Calculate new width
	length = x2 - x + 1 ;
	
	// Draw the line
	memSet( this->bmp + x + y * this->width , color , length );
}

void _bitmap::drawHorizontalDottedLine( _coord x , _coord y , _length length , _pixel color )
{
	// Get end point of rect to draw
	_coord x2 = x + length - 1;
	_coord y2 = y;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
		
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

void _bitmap::drawFilledRect( _coord x , _coord y , _length w , _length h , _pixel color )
{	
	// Get end point of rect to draw
	_coord x2 = x + w - 1;
	_coord y2 = y + h - 1;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
		
	// Calculate new width/height
	w = x2 - x + 1;
	h = y2 - y + 1;
	
	_pixelArray to = this->bmp + y * this->width + x;
	
	if( w == this->width )
		memSet( to , color , w * h ); // optimize Algorithm for drawing on whole bitmap
	else
		// Draw the rectangle
		while( h-- )
		{
			memSet( to , color , w );
			to += this->width;
		}
}

void _bitmap::drawVerticalGradient( _coord x , _coord y , _length w , _length h , _pixel fromColor , _pixel toColor )
{
	
	if( fromColor == toColor ){
		this->drawFilledRect( x , y , w , h , fromColor );
		return;
	}
	
	_length gradHeight = h;
	_coord gradY = y;
	
	// Get end point of rect to draw
	_coord x2 = x + w - 1;
	_coord y2 = y + h - 1;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
	
	
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
			*temp++ = ( ( curR >> 11 ) & ( 31 << 0  ) ) // Adjust and bitwise and width 31
					| ( ( curG >> 6  ) & ( 31 << 5  ) )
					| ( ( curB >> 1  ) & ( 31 << 10 ) )
					| ( ( 1 << 15 ) );
			// Faster because of less bitshifts
		
	// Draw the rectangle
	for ( _u32 i = 0 ; i != h; i++ )
		this->blitFill( x , y + i , gradTable[dY+i] , w );
	
	delete[] gradTable;
}

void _bitmap::drawHorizontalGradient( _coord x , _coord y , _length w , _length h , _pixel fromColor , _pixel toColor )
{
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
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
	
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
	_u16 part = this->width - ( w + x );
	
	// Fill the rect
	while( h-- )
	{
		temp = start;
		while( temp != end )
			*pos++ = *temp++;
		pos += part;
	}
	
	delete[] gradTable;
}

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

void _bitmap::drawString( _coord x0 , _coord y0 , _font* font , string str , _pixel color , _u8 fontSize )
{
	// Check for transparent
	if( !RGB_GETA(color) )
		return;
	
	for( const _char& ch : str )
	{
		if( x0 > this->activeClippingRect.getX2() )
			break;
		x0 += 1 + this->drawChar( x0 , y0 , font , ch , color , fontSize );
	}
}

void _bitmap::copy( _coord x , _coord y , const _bitmap& data )
{
	if( !data.isValid() )
		return;
	
	_length dataWidth = data.getWidth();
	_coord x2 = x + dataWidth - 1;
	_coord y2 = y + data.getHeight() - 1;
	_coord origX = x;
	_coord origY = y;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
	
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
	if( !data.isValid() )
		return;
	
	_coord x2 = x + data.getWidth() - 1;
	_coord y2 = y + data.getHeight() - 1;
	_coord origX = x;
	_coord origY = y;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
	
	_pixelArray copyData = data.getBitmap( x - origX , y - origY );
	_pixelArray myData = this->getBitmap( x , y );
	
	_length h = y2 - y + 1;
	_length w = x2 - x + 1;
	
	if( h <= 0 || w <= 0 )
		return;
		
	_int j = w;
	
	int dataDiff = data.getWidth() - w;
	int myDiff = this->width - w;
	
	while(true)
	{
		unsigned short val = *copyData++;
		if( val >> 15 ) // Check if this pixel of the data is non-transparent
			*myData = val;
		myData++;
		
		if( !--j )
		{
			copyData += dataDiff;
			myData += myDiff;
			j = w;
			if( !--h )
				break;
		}
	}
}

void _bitmap::copyHorizontalStretch( _coord x , _coord y , _length w , const _bitmap& data )
{
	_coord x2 = x + w - 1;
	_coord y2 = y + data.getHeight() - 1;
	_coord origY = y;
	
	if( !data.isValid() )
		return;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
	
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
	_coord x2 = x + data.getWidth() - 1;
	_coord y2 = y + h - 1;
	_coord origX = x;
	
	if( !data.isValid() )
		return;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
	
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
			
			delete buffer;
			
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
	
	if( !this->activeClippingRect.isValid() )
		return false;
	
	_coord x = activeClippingRect.x;
	
	// Ensure values don't exceed clipping rectangle
	left 	= max( left , x );
	right 	= min( right , _coord( x + _coord( activeClippingRect.width ) - 1 ) );
	
	if( right < left )
		return false;
	
	_coord y = activeClippingRect.y;
	
	top 	= max( top , y );
	bottom 	= min( bottom , _coord( y + _coord( activeClippingRect.height ) - 1 ) );
	
	// Return false if no box to draw
	// Return true as box can be drawn
	return bottom >= top;
}

_u16 _bitmap::drawChar( _coord x0 , _coord y0 , _font* font , _char ch , _pixel color , _u8 fontSize ){
	// Let the font do the hard work!
	return font->drawCharacter( this , x0 , y0 , ch , color , this->activeClippingRect , fontSize );
}

void _bitmap::blitFill( _coord x , _coord y , _pixel color , _length length ){
	memSet( &this->bmp[ y * this->width + x ] , color , length );
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
	// Just in case...
	if( this == &bmp )
		return *this;
	
	this->destruct();
	this->width = bmp.width;
	this->height = bmp.height;
	this->wasAllocated = bmp.wasAllocated;
	
	// Copy, if memory is owned by the bitmap or write, if just pointing to memory
	this->bmp = bmp.bmp;
	
	// Reset
	bmp.wasAllocated = false;
	bmp.bmp = nullptr;
	
	this->resetClippingRect();
	
	return *this;
}