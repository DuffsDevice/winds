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
	{
		newBmp = this->bmp;
	}
		
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
		memSet( newBmp , 0 , h * this->width );
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
		newBmp = new _pixel[ w * this->height ];
		memSet( newBmp , 0 , w * this->height );
	}
	else
		newBmp = this->bmp;
	
	_length minH = min( this->height , h );
	
	if( w < this->width )
	{
		for( _u32 y = 0; y != minH ; y++ )
			for( _u32 x = 0; x != w ; x++ )
				newBmp[ y * w + x ] = this->bmp[ y * this->width + x ];
	}
	else
	{
		for( _int y = minH ; y != 0 ; y-- )
			for( _int x = this->width ; x != 0 ; x-- )
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
	
	_coord clipX1 = x;
	_coord clipY1 = y;
	_coord clipX2 = x;
	_coord clipY2 = y;

	// Attempt to clip and draw
	if ( ! this->clipCoordinates( clipX1, clipY1, clipX2, clipY2 )) return;
	
	_u32 position = y * this->width + x;
	
	this->bmp[position] = color;
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
	
	// Pointer to BitMapBase
	_pixel* ptr = this->bmp + x + y * this->width;
	
	// Fill...
	for( _u32 i = 0; i != length; ++i , ptr += this->width )
		*ptr = color;
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
	for( _u32 i = 0; i != length; ++i , ptr += this->width << 2 )
		*ptr = color;
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
	this->blitFill( x, y, color, length );
	
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
	for( _u32 i = 0; i != length; ++i , ptr++ )
		*ptr++ = color;
	
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
	
	// Draw the rectangle
	while( h-- )
	{
		memSet( to ,  color , w );
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
		_pixelArray temp = gradTable + dY;
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
			*temp++ = RGB( curR >> 11 , curG >> 11 , curB >> 11 );
		
	// Draw the rectangle
	for ( _u32 i = 0; i != h; i++)
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
	_coord gradX = x;
	
	// Get end point of rect to draw
	_coord x2 = x + w - 1;
	_coord y2 = y + h - 1;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
	
	// Calculate new width/height
	w = x2 - x + 1;
	h = y2 - y + 1;
	int dX = x - gradX;
	
	//! Gradient Computations
		
		_pixelArray gradTable = new _pixel[w];
		_pixelArray start = gradTable + dX;
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
			*temp++ = RGB( curR >> 11 , curG >> 11 , curB >> 11 );
		
	// Draw Set Bitmap-starting-ptr
	_pixelArray pos = this->bmp + y * this->width + x;
	_u16 part = this->width - ( w + x );
	
	// Fill the rect
	for ( _u32 i = 0; i != h; ++i )
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

void _bitmap::copy( _coord x , _coord y , const _bitmap* data )
{
	if( !data->isValid() )
		return;
	
	_coord x2 = x + data->getWidth() - 1;
	_coord y2 = y + data->getHeight() - 1;
	_coord origX = x;
	_coord origY = y;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
	
	_pixelArray copyData = data->getBitmap( x - origX , y - origY );
	
	if( !copyData )
		return;
	
	_pixelArray myData	= this->getBitmap( x , y );
	
	_length h = y2 - y + 1;
	_length w = x2 - x + 1;
	
	while( h-- )
	{
		memCpy( myData , copyData , w );
		copyData += data->getWidth();
		myData += this->width;
	}
}

void _bitmap::copyTransparent( _coord x , _coord y , const _bitmap* data )
{
	//asm volatile("@Here I Start");
	if( !data->isValid() )
		return;
	
	_coord x2 = x + data->getWidth() - 1;
	_coord y2 = y + data->getHeight() - 1;
	_coord origX = x;
	_coord origY = y;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
	
	_pixelArray copyData = data->getBitmap( x - origX , y - origY );
	if( !copyData )
		return;
	_pixelArray myData	= this->getBitmap( x , y );
	
	_length h = y2 - y + 1;
	_length w = x2 - x + 1;
	int j;
	while( h-- )
	{
		j = w;
		while( j-- ){
			if( copyData[j] >> 15 ) // Check if this pixel of the data is non-transparent
				myData[j] = copyData[j];
		}
		copyData += data->getWidth();
		myData += this->width;
	}
}

void _bitmap::copyHorizontalStretch( _coord x , _coord y , _length w , const _bitmap* data )
{
	_coord x2 = x + w - 1;
	_coord y2 = y + data->getHeight() - 1;
	_coord origY = y;
	
	if( !data->isValid() )
		return;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
	
	_pixelArray copyData = data->getBitmap();
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

void _bitmap::copyVerticalStretch( _coord x , _coord y , _length h , const _bitmap* data )
{
	_coord x2 = x + data->getWidth() - 1;
	_coord y2 = y + h - 1;
	_coord origX = x;
	
	if( !data->isValid() )
		return;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
	
	// Get Data
	_pixelArray copyData = &data->operator[]( + x - origX );
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
		for ( _int i = height - 1 ; i != -1; --i , src -= this->width , dst -= this->width )
			memCpy( dst , src , width );
	}
}


bool _bitmap::clipCoordinates( _coord &left , _coord &top , _coord &right , _coord &bottom ) const 
{
	if( !activeClippingRect.isValid() )
		return false;
	
	// Ensure values don't exceed clipping rectangle
	left 	= max( left , activeClippingRect.x );
	top 	= max( top , activeClippingRect.y );
	right 	= min( _length( right ) , activeClippingRect.x + activeClippingRect.width - 1 );
	bottom 	= min( _length( bottom ) , activeClippingRect.y + activeClippingRect.height - 1 );
	
	// Return false if no box to draw
	if ( ( right < left ) || ( bottom < top ) ) return false;
	
	// Return true as box can be drawn
	return true;
}

_u16 _bitmap::drawChar( _coord x0 , _coord y0 , _font* font , _char ch , _pixel color , _u8 fontSize ){
	// Let the font do the hard work!
	return font->drawCharacter( this , x0 , y0 , ch , color , this->activeClippingRect , fontSize );
}

void _bitmap::blitFill( _coord x , _coord y , _pixel color , _length length ){
	memSet( &this->bmp[ y * this->width + x ] , color , length );
}

void _bitmap::setClippingRect( _rect rc ){
	this->activeClippingRect = rc;
	_coord dX = max( -this->activeClippingRect.x , 0 );
	_coord dY = max( -this->activeClippingRect.y , 0 );
	this->activeClippingRect.x += dX;
	this->activeClippingRect.width -= dX;
	this->activeClippingRect.y += dY;
	this->activeClippingRect.height -= dY;
	this->activeClippingRect.setX2( min( this->activeClippingRect.getX2() , _coord( this->width ) ) );
	this->activeClippingRect.setY2( min( this->activeClippingRect.getY2() , _coord( this->height ) ) );
}

_bitmap& _bitmap::operator=( const _bitmap& bmp )
{
	if( this == &bmp )
		return *this;
	this->destruct();
	this->width = max( _length( 1 ) , bmp.width );
	this->height = max( _length( 1 ) , bmp.height );
	this->activeClippingRect = bmp.activeClippingRect;
	this->wasAllocated = true;
	this->bmp = new _pixel[this->width*this->height];
	memCpy( this->bmp , bmp.bmp , this->width*this->height );
	return *this;
}