#include "_type/type.bitmap.h"


_bitmap::_bitmap( _pixelArray base , _length w , _length h )
	: bmp( base ) , width( w ), height( h ) , wasAllocated( false )
{ this->resetClippingRect(); }

_bitmap::_bitmap( _length w , _length h )
	: width( w ), height( h ) , wasAllocated( true )
{
	this->bmp = new _pixel[w*h];
	this->resetClippingRect();
}

_bitmap::_bitmap( const _bitmap &bm )
	: bmp( bm.getBitmap() ) , width( bm.getWidth() ), height( bm.getHeight() ) , wasAllocated( false )
{ this->resetClippingRect(); }

_bitmap::_bitmap()
	: bmp( nullptr ) , width( 0 ), height( 0 ) , wasAllocated( false )
{ this->resetClippingRect(); }

_pixelArray _bitmap::getBitmap() const {
	return this->bmp;
}

_pixelArray _bitmap::getBitmap( _length y ) const {
	return &this->bmp[ y * this->width ];
}

_pixelArray _bitmap::getBitmap( _length x , _length y ) const {
	return &this->bmp[ y * this->width + x ];
}

void _bitmap::setBitmap( _pixelArray bmp ) {
	this->bmp = bmp;
}

_pixel& _bitmap::operator[]( const _u32 pos ) const {
	return this->bmp[ min( _u32(this->width * this->height - 1 ) , pos ) ];
}

_pixel& _bitmap::operator()( _coord x , _coord y ) const {
	
	x = min( _length(x) , this->width - 1 );
	y = min( _length(y) , this->height -1 );
	
	_u32 position = y * this->width + x;
	
	return this->bmp[position];
}

_length _bitmap::getWidth() const {
	return this->width;
}

_length _bitmap::getHeight() const {
	return this->height;
}

void _bitmap::setWidth( _length w )
{ 
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
		for( _u16 y = 0; y < this->height ; y++ )
		{
			for( _u16 x = 0; x < w ; x++ )
			{
				newBmp[ y * w + x ] = this->bmp[ y * this->width + x ];
			}
		}
	}
	else
	{
		for( _u16 y = this->height ; y > 0 ; y-- )
		{
			for( _u16 x = this->width ; x > 0 ; x-- )
			{
				newBmp[ ( y - 1 ) * w + x - 1 ] = this->bmp[ ( y - 1 ) * this->width + x - 1 ];
			}
		}
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
	{
		newBmp = this->bmp;
	}
	
	memCopy( newBmp , this->bmp , min( this->height , h ) * this->width );
	
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
	
	_length minH = min( this->height , h );
	
	if( w < this->width )
	{
		for( _u16 y = 0; y < minH ; y++ )
		{
			for( _u16 x = 0; x < w ; x++ )
			{
				newBmp[ y * w + x ] = this->bmp[ y * this->width + x ];
			}
		}
	}
	else
	{
		for( _u16 y = minH ; y > 0 ; y-- )
		{
			for( _u16 x = this->width ; x > 0 ; x-- )
			{
				newBmp[ ( y - 1 ) * w + x - 1 ] = this->bmp[ ( y - 1 ) * this->width + x - 1 ];
			}
		}
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

void _bitmap::destruct(){ 
	if( this->wasAllocated ){
		delete[] this->bmp;
	}
}

_bitmap::~_bitmap(){ 
	this->destruct();
}

_pixel _bitmap::getPixel( _coord x , _coord y ) const 
{
	_u32 position = y * this->width + x;
	
	// Prevent Overflows
	if( position > (this->height * this->width - 1 ) || x >= this->width || y >= this->height )
		return NO_COLOR;
	
	return this->bmp[position];
}

_pixel _bitmap::getPixelFast( _coord x , _coord y ) const 
{
	return this->bmp[ y * this->width + x ];
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

void _bitmap::blitFill( _coord x , _coord y , _pixel color , _length length )
{
	//dmaFillHalfWords( color , &this->bmp[this->width*y+x] , sizeof(_pixel) * length );
	memSet( &this->bmp[ y * this->width + x ] , color , length );
}

void _bitmap::drawPixelNoCheck( _coord x , _coord y , _pixel color )
{
	this->bmp[y * this->width + x] = color;
}

void _bitmap::reset( _pixel color )
{
	this->blitFill( 0 , 0 , color , this->width * this->height );
}

void _bitmap::fill( _pixel color )
{
	this->drawFilledRect( 0 , 0 , this->width , this->height , color );
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
	for(int i = 0; i < length; ++i , ptr += this->width )
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

void _bitmap::drawRect( _coord x , _coord y , _length w , _length h , _pixel color )
{			
	--w;
	--h;
	
	this->drawVerticalLine( x , y , h , color );
	this->drawVerticalLine( x + w  , y + 1 , h , color );
	this->drawHorizontalLine( x + 1, y , w , color );
	this->drawHorizontalLine( x , y + h  , w , color );
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
		
	// Draw the rectangle
	for (_u16 i = 0; i < h; i++) {
		this->blitFill( x , y + i , color , w );
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
	_pixelArray gradTable = new _pixel[h];
	
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
		
		if( dY > 0 ){
			curR += trigR*dY; curG += trigG*dY; curB += trigB*dY;
		}
		
		// Fill the table
		for( int t = 0; t < h ; t++, curR += trigR, curG += trigG, curB += trigB ){
			gradTable[ dY + t ] = RGB( curR >> 11 , curG >> 11 , curB >> 11 );
		}
		
	// Draw the rectangle
	for (_coord i = 0; i < h; i++)
		this->blitFill(x, y + i, gradTable[dY+i] , w );
	
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
	_pixelArray gradTable = new _pixel[w];
	
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
		int curR = RGB_GETR(fromColor) << 11;
		int curG = RGB_GETG(fromColor) << 11;
		int curB = RGB_GETB(fromColor) << 11;
		
		if( dX > 0 ){
			curR += trigR*dX; curG += trigG*dX; curB += trigB*dX;
		}
		
		// Fill the table
		for( int t = 0; t < w ; t++, curR += trigR, curG += trigG, curB += trigB ){
			gradTable[dX+t] = RGB( curR >> 11 , curG >> 11 , curB >> 11 );
		}
		
	// Draw one line!
	for (_coord i = 0; i < w; i++)
		this->drawPixelNoCheck( x + i , y, gradTable[dX+i] );
	
	// Copy that line!
	for (_coord i = 1; i < h; i++){
		memCopy( &this->bmp[ this->width * ( y + i ) + x ] , &this->bmp[ y * this->width + x ] , w );
		//dmaCopyWords( 3 , &this->bmp[ y * this->width + x ] , &this->bmp[ this->width * ( y + i ) + x ] , sizeof(_pixel) * w );
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

	while (y>=0 && x<=a) {
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

	while (y>=0 && x<=a) {
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

_u16 _bitmap::drawChar( _coord x0 , _coord y0 , _font* font , _char ch , _pixel color , _u8 fontSize )
{
	// Let the font do the hard work!
	return font->drawCharacter( this , x0 , y0 , ch , color , this->activeClippingRect , fontSize );
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
	if( data->getWidth() == 0 || data->getHeight() == 0 )
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
	
	for(int i = 0; i < h; i++ )
	{
		memCopy( myData , copyData , w );
		copyData += data->getWidth();
		myData += this->width;
	}
}

void _bitmap::copyTransparent( _coord x , _coord y , const _bitmap* data , _pixel transparentColor )
{
	
	if( data->getWidth() == 0 || data->getHeight() == 0 )
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
	
	for(int i = 0; i < h; i++ )
	{
		for( int j = 0; j < w; j++ ){
			if( RGB_GETA( copyData[j] ) && copyData[j] != transparentColor ) // Check if this pixel of the data is non-transparent
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
	
	if( data->getWidth() == 0 || data->getHeight() == 0 )
		return;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
	
	_pixelArray copyData = data->getBitmap();
	if( !copyData )
		return;
	_pixelArray myData	= &this->bmp[ y * this->width + x ];
	
	_length height = y2 - y + 1;
	_length width = x2 - x + 1;
	
	for(int i = 0; i < height; i++ )
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
	
	if( data->getWidth() == 0 || data->getHeight() == 0 )
		return;
	
	// Attempt to clip
	if ( ! this->clipCoordinates( x ,  y , x2 , y2 ) ) return;
	
	// Get Data
	_pixelArray copyData = &data->operator[]( + x - origX );
	if( !copyData )
		return;
	_pixelArray myData	= &this->bmp[ y * this->width + x ];
	
	_length height = y2 - y + 1;
	_length width = x2 - x + 1;
	
	// Draw one line!
	memCopy( myData , copyData , width );
	
	_pixelArray destination = myData;
	
	// Copy that line!
	for (_u16 i = 1; i < height; i++){
		destination += this->width;
		memCopy( destination , myData , width );
	}
}

bool _bitmap::clipCoordinates( _coord &left , _coord &top , _coord &right , _coord &bottom )
{
	
	if( !activeClippingRect.isValid() )
		return false;
		
	// Get co-ords of clipping rect
	_coord minX = activeClippingRect.x;
	_coord minY = activeClippingRect.y;
	_coord maxX = activeClippingRect.x + activeClippingRect.width - 1;
	_coord maxY = activeClippingRect.y + activeClippingRect.height - 1;

	// Choose larger start point values
	minX = max( (int)minX , 0 );
	minY = max( (int)minY , 0 );
	
	// Choose smaller end point values
	maxX = min( maxX , _coord( this->width - 1 ) );
	maxY = min( maxY , _coord( this->height - 1 ) );
	
	// Ensure values don't exceed clipping rectangle
	left 	= max( left , minX );
	top 	= max( top , minY );
	right 	= min( right , maxX );
	bottom 	= min( bottom , maxY );
	
	// Return false if no box to draw
	if ( ( right < left ) || ( bottom < top ) ) return false;
	
	// Return true as box can be drawn
	return true;
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

_rect _bitmap::getClippingRect(){
	return this->activeClippingRect;
}

void _bitmap::resetClippingRect(){
	this->activeClippingRect = _rect( 0 , 0 , this->width , this->height );
}
