// Check if already included
#ifndef _WIN_T_BITMAP_
#define _WIN_T_BITMAP_

#include "_type/type.h"
#include "_type/type.font.h"
#include "_type/type.rect.h"
#include "_type/type.color.h"
#include "func.memory.h"

class _font;

//! A Wrapper-Class to store a BMP-Field
class _bitmap
{
	protected:
	
		_pixelArray	bmp;
		
		_length		width;
		_length		height;
		
		_rect		activeClippingRect;
		
		bool		wasAllocated;
		
		/**
		 * Manual Data-Erase
		 * ( only deletes the Data if it was allocated by the bitmap itself)
		 * @return void
		 */
		void destruct(){ 
			if( this->wasAllocated )
				delete[] this->bmp;
		}
	
	public:
		
		/**
		 * Construcor: Make Bitmap out of the data, type and dimensions
		 * @param base Base-Bitmap (_pixelArray) to use
		 * @param w Width of the bmp
		 * @param h Height of the bmp
		 * @return void
		 */
		_bitmap( _pixelArray base , _length w , _length h ) :
			bmp( base )
			, width( max( (_length)1 , w ) )
			, height( max( (_length)1 , h ) )
			, wasAllocated( false )
		{
			this->resetClippingRect();
		}
		
		/**
		 * Construcor: Make Bitmap out of dimensions (data will be allocated automatically)
		 * @param w Width of the bmp
		 * @param h Height of the bmp
		 * @return void
		 */
		_bitmap( _length w , _length h ) :
			width( max( (_length)1 , w ) )
			, height( max( (_length)1 , h ) )
			, wasAllocated( true )
		{
			this->bmp = new _pixel[w*h];
			this->resetClippingRect();
		}
		
		/**
		 * Construcor: Make Bitmap out of dimensions (data will be allocated automatically) and resetted to the passed color
		 * @param w Width of the bmp
		 * @param h Height of the bmp
		 * @param col Initial Color of the bitmap
		 * @return void
		 */
		_bitmap( _length w , _length h , _color col ) :
			_bitmap( w , h )
		{
			memSet( this->bmp , col , this->width * this->height );
		}
		
		/**
		 * Copy-Constructor
		 * @param bm Source Bitmap
		 * @return void
		 */
		_bitmap( const _bitmap &bm ) :
			bmp( bm.wasAllocated ? new _pixel[bm.width*bm.height] : bm.bmp )
			, width( bm.width )
			, height( bm.height )
			, wasAllocated( bm.wasAllocated )
		{
			if( wasAllocated )
				memCpy( bmp , bm.bmp , width * height );
		}
		
		/**
		 * Move-Constructor
		 * @param bm Source Bitmap
		 * @return void
		 */
		_bitmap( _bitmap &&bm )
			: bmp( bm.bmp )
			, width( bm.width )
			, height( bm.height )
			, wasAllocated( bm.wasAllocated )
		{
			//! Move!
			bm.wasAllocated = false;
			bm.bmp = nullptr;
			this->resetClippingRect();
		}
		
		/**
		 * Default constructor
		 * @return void
		 */
		_bitmap() :
			bmp( nullptr )
			, width( 0 )
			, height( 0 )
			, wasAllocated( false )
		{}
		
		/**
		 * Destructor
		 * @return void
		 */
		~_bitmap(){
			this->destruct();
		}
		
		/**
		 * Check if a bitmap has valid attributes
		 * @return bool
		 */
		bool isValid() const { return this->bmp != nullptr; }
		
		/**
		 * Copy Bitmap (copy its data onto mine)
		 * @param bmp Source Bitmap
		 * @return _bitmap
		 */
		_bitmap& operator=( const _bitmap& bmp );
		
		/**
		 * Move Bitmap
		 * @param bmp Source Bitmap
		 * @return _bitmap
		 */
		_bitmap& operator=( _bitmap&& bmp );
		
		/**
		 * Get the Bitmap Base
		 * @return _pixelArray Pointer to the bitmap data
		 */
		_pixelArray getBitmap() const {
			return this->bmp;
		}
		
		/**
		 * Get the Bitmap Base starting at a specific Position
		 * @param y Row to get
		 * @return _pixelArray Pointer to the bitmap data
		 */
		_pixelArray getBitmap( _length y ) const {
			return &this->bmp[ y * this->width ];
		}
		
		/**
		 * Get the Bitmap Base starting at a specific Position
		 * @param x Col to get
		 * @param y Row to get
		 * @return _pixelArray Pointer to the bitmap data
		 */
		_pixelArray getBitmap( _length x , _length y ) const {
			return &this->bmp[ y * this->width + x ];
		}
		
		/**
		 * Set the Bitmap Base
		 * @param bmp Pointer to the bitmap data
		 */
		void setBitmap( _pixelArray bmp ){
			this->destruct();
			this->wasAllocated = false;
			this->bmp = bmp;
		}
		
		/**
		 * Operator for [i] to get a specific position of the bmp
		 *
		 */
		_pixel& operator[]( const _u32 pos ) const {
			return this->bmp[ min( _u32( this->width * this->height - 1 ) , pos ) ];
		}
		
		/**
		 * Operator for [x][y] to get a specific pixel (x,y) of the bmp
		 *
		 */
		_pixel& operator()( _coord x , _coord y ) const
		{
			x = min( x , _coord( this->width - 1 ) );
			y = min( y , _coord( this->height - 1 ) );
			
			_u32 position = y * this->width + x;
			
			return this->bmp[position];
		}
		
		/**
		 * Get the Bitmap's Width
		 * @return u16 Width of the _bitmap
		 */
		_length getWidth() const {
			return this->width;
		}
		
		/**
		 * Get the Bitmap's Height
		 * @return u16 Height of the _bitmap
		 */
		_length getHeight() const {
			return this->height;
		}
		
		/**
		 * Set the Bitmap's Width
		 * @param w Width of the _bitmap
		 */
		void setWidth( _length w );
		
		/**
		 * Set the Bitmap's Height
		 * @param h Height of the _bitmap
		 */
		void setHeight( _length h );
		
		/**
		 * Resize the Bitmap
		 * @param w Width of the _bitmap
		 * @param h Height of the _bitmap
		 */
		void resize( _length w , _length h );
		
		/**
		 * Get the Pixel at a specific location
		 * @param x X-Position to check
		 * @param y Y-Position to check
		 * @return _color The Pixel at the specified location (if not foound: _color::transparent)
		 */
		_color getPixel( _coord x , _coord y ) const 
		{			
			// Prevent Overflows
			if( !this->isValid() || x >= _coord( this->width ) || y >= _coord( this->height ) )
				return _color::transparent;
			
			return this->bmp[y * this->width + x];
		}
		
		/**
		 * Get the Pixel at a specific location, but without a check if the poition is inside the bmp
		 * @param x X-Position to check
		 * @param y Y-Position to check
		 * @return _color The Pixel at the specified location
		 */
		private:
		_color getPixelUnsafe( _coord x , _coord y ) const {
			return this->bmp[ y * this->width + x ];
		}
		public:
		
		/**
		 * Set the Pixel at a specific location
		 * @param x X-Position
		 * @param y Y-Position
		 * @param color Color of the Pixel to set
		 * @return void
		 */
		void drawPixel( _coord x , _coord y , _color color )
		{
			if( activeClippingRect.contains( x , y ) )
				this->bmp[y * this->width + x] = color;
		}
		
		/**
		 * Set the Pixel at a specific location, but without a check if the poition is inside the bmp
		 * @param x X-Position
		 * @param y Y-Position
		 * @param color Color of the Pixel to set
		 * @return void
		 */
		private:
		void drawPixelUnsafe( _coord x , _coord y , _color color ){
			this->bmp[y * this->width + x] = color;
		}
		public:
		
		/**
		 * Fill a part of the memory with the color supplied
		 * @param x X-Position
		 * @param y Y-Position
		 * @param length Length of the Memory-Block to be filled
		 * @param color Color to fill
		 * @return void
		 */
		private:
		void blitFill( _coord x , _coord y , _color color , _length length ){
			if( this->isValid() ) memSet( &this->bmp[ y * this->width + x ] , color , length );
		}
		public:
		
		/**
		 * Erase the whole bmp
		 * @param color Optionally: The Color to erase
		 * @return void
		 */
		void reset( _color color = _color::transparent ){
			this->blitFill( 0 , 0 , color , this->width * this->height );
		}
		
		/**
		 * Fill the whole bmp
		 * @param color The Color to fill with
		 * @return void
		 */
		void fill( _color color ){
			this->drawFilledRect( 0 , 0 , this->width , this->height , color );
		}
		
		/**
		 * Replace all pixels of a certain color with another
		 * @param color The Color to replace
		 * @param replace The Color to replace with
		 * @return void
		 */
		void replaceColor( _color color , _color replace );
		
		/**
		 * Draw a vertical Line onto the bmp
		 * @param x X-Position
		 * @param y Y-Position Start (top)
		 * @param length Length of the Line (height)
		 * @param color Color of the Line
		 * @return void
		 */
		void drawVerticalLine( _coord x , _coord y , _length length , _color color );
		
		/**
		 * Draw a vertical dotted Line onto the bmp
		 * @param x X-Position
		 * @param y Y-Position Start (top)
		 * @param length Length of the Line (height)
		 * @param color Color of the Line
		 * @return void
		 */
		void drawVerticalDottedLine( _coord x , _coord y , _length length , _color color );
		
		/**
		 * Draw a horizontal Line onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start
		 * @param length Length of the Line (width)
		 * @param color Color of the Line
		 * @return void
		 */
		void drawHorizontalLine( _coord x , _coord y , _length length , _color color );
		
		/**
		 * Draw a horizontal dotted Line onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start
		 * @param length Length of the Line (width)
		 * @param color Color of the Line
		 * @return void
		 */
		void drawHorizontalDottedLine( _coord x , _coord y , _length length , _color color );
		
		/**
		 * Draw a horizontal Line onto the bmp
		 * @param x1 Start-X-Position
		 * @param y1 Start-Y-Position
		 * @param x2 End-X-Position
		 * @param y2 End-Y-Position
		 * @param color Color of the Line
		 * @return void
		 */
		void drawLine( _coord x1 , _coord y1 , _coord x2 , _coord y2 , _color color );
		private:
		void drawClippedLine( _coord x1 , _coord y1 , _coord x2 , _coord y2 , _color color );
		static _u8 getClipLineOutCode( _coord x , _coord y , _coord xMin , _coord yMin , _coord xMax , _coord yMax );
		public:
		
		/**
		 * Draw a Rectangle onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start (top)
		 * @param width Width of the Rectangle (1 means: 1Pixel in width)
		 * @param height Height of the Rectangle (1 means: 1Pixel in height)
		 * @param color Color of the Rect
		 * @return void
		 */
		void drawRect( _coord x , _coord y , _length w , _length h , _color color );
		
		/**
		 * Draw a filled Rectangle onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start (top)
		 * @param width Width of the Rectangle (1 means: 1Pixel in width)
		 * @param height Height of the Rectangle (1 means: 1Pixel in height)
		 * @param color Color of the Rect
		 * @return void
		 */
		void drawFilledRect( _coord x , _coord y , _length width , _length height , _color color );
		
		/**
		 * Draw a dotted Rectangle onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start (top)
		 * @param width Width of the Rectangle (1 means: 1Pixel in width)
		 * @param height Height of the Rectangle (1 means: 1Pixel in height)
		 * @param color Color of the Rect
		 * @return void
		 */
		void drawDottedRect( _coord x , _coord y , _length w , _length h , _color color );
		
		/**
		 * Draw a Gradient onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start (top)
		 * @param width Width of the Rectangle (1 means: 1Pixel in width)
		 * @param height Height of the Rectangle (1 means: 1Pixel in height)
		 * @param fromColor Start Color (top)
		 * @param toColor End Color (bottom)
		 * @return void
		 */
		void drawVerticalGradient( _coord x , _coord y , _length width , _length height , _color fromColor , _color toColor );
		
		/**
		 * Draw a Gradient onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start (top)
		 * @param width Width of the Rectangle (1 means: 1Pixel in width)
		 * @param height Height of the Rectangle (1 means: 1Pixel in height)
		 * @param fromColor Start Color (top)
		 * @param toColor End Color (bottom)
		 * @return void
		 */
		void drawHorizontalGradient( _coord x , _coord y , _length width , _length height , _color fromColor , _color toColor );
		
		/**
		 * Draw a Circle onto the bmp
		 * @param xc X-Position Center
		 * @param yc Y-Position Center
		 * @param radius Radius of the Circle
		 * @param color Color of the Circle
		 * @return void
		 */
		void drawCircle( _coord xc, _coord yc, _length radius, _color color);
		
		/**
		 * Draw a filled Circle onto the bmp
		 * @param xc X-Position Center
		 * @param yc Y-Position Center
		 * @param radius Radius of the Circle
		 * @param color Color of the Circle
		 * @return void
		 */
		void drawFilledCircle( _coord xc, _coord yc, _length radius, _color color);
		
		/**
		 * Draw an Ellipse onto the bmp
		 * @param xc X-Position Center
		 * @param yc Y-Position Center
		 * @param a Semi-Major Axis (in pixels)(=>Große Bahnhalbachse)
		 * @param b Semi-Minor Axis (in pixels)(=>Kleine Bahnhalbachse)
		 * @param color Color of the Ellipse
		 * @return void
		 * @see 99392 ticks on dimensions 256*192
		 */
		void drawFilledEllipse( _coord xc, _coord yc, _length a, _length b, _color color);
		
		/**
		 * Draw an Ellipse onto the bmp
		 * @param xc X-Position Center
		 * @param yc Y-Position Center
		 * @param a Semi-Major Axis (in pixels)(=>Große Bahnhalbachse)
		 * @param b Semi-Minor Axis (in pixels)(=>Kleine Bahnhalbachse)
		 * @param color Color of the Ellipse
		 * @return void
		 */
		void drawEllipse( _coord xc, _coord yc, _length a, _length b, _color color);
		
		/**
		 * Draw a single _character to a specific Position and returns the width it used
		 * @param x0 X-Position Left
		 * @param y0 Y-Position Top
		 * @param font The _font to use
		 * @param ch _character to draw (ASCII)
		 * @param color Color of the _character
		 * @return int The Width of the _character it has drawn
		 */
		_u16 drawChar( _coord x0 , _coord y0 , const _font* font , _char ch , _color color , _u8 fontSize = 0 )
		{
			// Check if everything is valid
			if( !this->isValid() || !font || !font->isValid() ) 
				return 0;
			
			return drawCharUnsafe( x0 , y0 , font , ch , color , fontSize );
		}
		private:
		_u16 drawCharUnsafe( _coord x0 , _coord y0 , const _font* font , _char ch , _color color , _u8 fontSize = 0 )
		{
			// Fetch the destination where to draw To
			_pixelArray dest = & this->bmp[ y0 * this->width + x0 ];
			
			// Let the font do the hard work!
			return font->drawCharacter( dest , this->width , x0 , y0 , ch , color , this->activeClippingRect , fontSize );
		}
		public:
		
		/**
		 * Draw a String to a specific Position
		 * @param x0 X-Position Left
		 * @param y0 Y-Position Top
		 * @param font The _font to use
		 * @param str The String to draw
		 * @param color Color of the String
		 * @return void
		 */
		void drawString( _coord x0 , _coord y0 , const _font* font , const _char* str , _color color , _u8 fontSize = 0 );
		void drawString( _coord x0 , _coord y0 , const _font* font , string str , _color color , _u8 fontSize = 0 )
		{
			drawString( x0 , y0 , font , str.c_str() , color , fontSize );
		}
		
		/**
		 * Copy a _bitmap onto the bitmap
		 * @param x 		X-Position Left
		 * @param y 		Y-Position Top
		 * @param data 		Other _bitmap
		 * @param bitmapX 	X-Origin of copying on the _bitmap (optional)
		 * @param bitmapY 	Y-Origin of copying on the _bitmap (optional)
		 * @return void
		 */
		void copy( _coord x , _coord y , const _bitmap& data );
		
		/**
		 * Copy a _bitmap onto the bitmap but with respecting the alpha channel
		 * @param x X-Position Left
		 * @param y Y-Position Top
		 * @param data Other _bitmap
		 * @return void
		 */
		void copyTransparent( _coord x , _coord y , const _bitmap& data );
		
		/**
		 * Copy a _bitmap onto the bitmap by taking the line x=0 and stretching that line over width
		 * @param x X-Position Left
		 * @param y Y-Position Top
		 * @param width Width to stretch it to
		 * @return void
		 */
		void copyHorizontalStretch( _coord x , _coord y , _length width , const _bitmap& data );
		
		/**
		 * Copy a _bitmap onto the bitmap by taking the row y=0 and stretching that line over width
		 * @param x X-Position Left
		 * @param y Y-Position Top
		 * @param height Height to stretch it to
		 * @return void
		 */
		void copyVerticalStretch( _coord x , _coord y , _length height , const _bitmap& data );
		
		/**
		 * Move a part of the bitmap to another location
		 * 
		 */
		void move( _coord sourceX , _coord sourceY , _coord destX , _coord destY , _length width , _length height );
		
		/**
		 * Set the active ClippingRect with bounds checking
		 * @param rc Rect The rect to be clipped to
		 * @return void
		 */
		void setClippingRect( _rect rc ){
			this->setClippingRectUnsafe( (_rect&&)rc.clipToIntersect( _rect( 0 , 0 , this->width , this->height ) ) );
		}
		
		
		/**
		 * Set the active ClippingRect without clipping that one to the dimensions of the bitmap
		 + @note Hence, the _rect MUST be COMPLETELY inside the bitmaps dimensions
		 * @param rc Rect The rect to be clipped to
		 * @return void
		 */
		void setClippingRectUnsafe( _rect rc ){ this->activeClippingRect = (_rect&&)rc; }
		
		/**
		 * Get the active ClippingRect
		 * @return _rect
		 */
		_rect getClippingRect(){
			return this->activeClippingRect;
		}
		
		/**
		 * Make the _bitmap not checking Coordinates
		 * @return void
		 */
		void resetClippingRect(){
			this->activeClippingRect.x = 0;
			this->activeClippingRect.y = 0;
			this->activeClippingRect.width = this->width;
			this->activeClippingRect.height = this->height;
		}
		
		/**
		 * Check if a Rectangle specified by its limits is visible if it gets clipped by the activeClippingRect
		 * @param s16 left Left side of the Rectangle to check
		 * @param s16 top Top side of the Rectangle to check
		 * @param s16 right Right side of the Rectangle to check
		 * @param s16 bottom Bottom side of the Rectangle to check
		 * @return bool Whether it is visible (true) or not (false)
		 */
		private:
		bool clipCoordinates( _coord &left , _coord &top , _coord &right , _coord &bottom ) const ;
		bool clipCoordinatesX( _coord &left , _coord &top , _coord& right ) const ;
		bool clipCoordinatesY( _coord &left , _coord &top , _coord& bottom ) const ;
		public:
};

typedef const _bitmap _constBitmap;

#endif