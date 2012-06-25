// Check if already included
#ifndef _WIN_T_BITMAP_
#define _WIN_T_BITMAP_

#include "_type/type.h"
#include "_type/type.font.h"
#include "_type/type.rect.h"
#include "func.memory.h"
#include <string.h>

#include "nds/arm9/math.h"

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
	
	public:
		
		/**
		 * Construcor: Make Bitmap out of the data, type and dimensions
		 * @param base Base-Bitmap (_pixelArray) to use
		 * @param w Width of the bmp
		 * @param h Height of the bmp
		 * @return void
		**/
		_bitmap( _pixelArray base , _length w , _length h );
		
		/**
		 * Construcor: Make Bitmap out of dimensions (data will be allocated automatically)
		 * @param w Width of the bmp
		 * @param h Height of the bmp
		 * @return void
		**/
		_bitmap( _length w , _length h );
		
		/**
		 * Copy-Constructor
		 * @param bm Source Bitmap
		 * @return void
		**/
		_bitmap( const _bitmap &bm );
		
		/**
		 * Destructor
		 * @return void
		**/
		~_bitmap();
		
		/**
		 * Manual Data-Erase
		 * ( only deletes the Data if it was allocated by the bitmap itself)
		 * @return void
		**/
		private:
		void destruct();
		public:
		
		/**
		 * Default constructor
		 * @return void
		**/
		_bitmap();
		
		/**
		 * Get the Bitmap Base
		 * @return _pixelArray Pointer to the bitmap data
		**/
		_pixelArray getBitmap() const ;
		
		/**
		 * Get the Bitmap Base starting at a specific Position
		 * @param y Row to get
		 * @return _pixelArray Pointer to the bitmap data
		**/
		_pixelArray getBitmap( _length y ) const ;
		
		/**
		 * Get the Bitmap Base starting at a specific Position
		 * @param x Col to get
		 * @param y Row to get
		 * @return _pixelArray Pointer to the bitmap data
		**/
		_pixelArray getBitmap( _length x , _length y ) const ;
		
		/**
		 * Set the Bitmap Base
		 * @param bmp Pointer to the bitmap data
		**/
		void setBitmap( _pixelArray bmp );
		
		/**
		 * Operator for [i] to get a specific position of the bmp
		 *
		**/
		_pixel& operator[]( const _u32 pos ) const ;
		
		/**
		 * Operator for [x][y] to get a specific pixel (x,y) of the bmp
		 *
		**/
		_pixel& operator()( _coord x , _coord y ) const ;
		
		/**
		 * Get the Bitmap's Width
		 * @return u16 Width of the _bitmap
		**/
		_length getWidth() const ;
		
		/**
		 * Get the Bitmap's Height
		 * @return u16 Height of the _bitmap
		**/
		_length getHeight() const ;
		
		/**
		 * Set the Bitmap's Width
		 * @param w Width of the _bitmap
		**/
		void setWidth( _length w );
		
		/**
		 * Set the Bitmap's Height
		 * @param h Height of the _bitmap
		**/
		void setHeight( _length h );
		
		/**
		 * Resize the Bitmap
		 * @param w Width of the _bitmap
		 * @param h Height of the _bitmap
		**/
		void resize( _length w , _length h );
		
		/**
		 * Get the Pixel at a specific location
		 * @param x X-Position to check
		 * @param y Y-Position to check
		 * @return _pixel The Pixel at the specified location (if not foound: NO_COLOR)
		**/
		_pixel getPixel( _coord x , _coord y ) const ;
		
		/**
		 * Get the Pixel at a specific location, but without a check if the poition is inside the bmp
		 * @param x X-Position to check
		 * @param y Y-Position to check
		 * @return _pixel The Pixel at the specified location
		**/
		private:
		_pixel getPixelFast( _coord x , _coord y ) const ;
		public:
		
		/**
		 * Set the Pixel at a specific location
		 * @param x X-Position
		 * @param y Y-Position
		 * @param color Color of the Pixel to set
		 * @return void
		**/
		void drawPixel( _coord x , _coord y , _pixel color );
		
		/**
		 * Fill a part of the memory with the color supplied
		 * @param x X-Position
		 * @param y Y-Position
		 * @param length Length of the Memory-Block to be filled
		 * @param color Color to fill
		 * @return void
		**/
		private:
		void blitFill( _coord x , _coord y , _pixel color , _length length );
		public:
		
		/**
		 * Set the Pixel at a specific location, but without a check if the poition is inside the bmp
		 * @param x X-Position
		 * @param y Y-Position
		 * @param color Color of the Pixel to set
		 * @return void
		**/
		private:
		void drawPixelNoCheck( _coord x , _coord y , _pixel color );
		public:
		
		/**
		 * Erase the whole bmp
		 * @param color Optionally: The Color to erase
		 * @return void
		**/
		void reset( _pixel color = BIT(15) );
		
		/**
		 * Fill the whole bmp
		 * @param color The Color to fill with
		 * @return void
		**/
		void fill( _pixel color );
		
		/**
		 * Draw a vertical Line onto the bmp
		 * @param x X-Position
		 * @param y Y-Position Start (top)
		 * @param length Length of the Line (height)
		 * @param color Color of the Line
		 * @return void
		**/
		void drawVerticalLine( _coord x , _coord y , _length length , _pixel color );
		
		/**
		 * Draw a horizontal Line onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start
		 * @param length Length of the Line (width)
		 * @param color Color of the Line
		 * @return void
		**/
		void drawHorizontalLine( _coord x , _coord y , _length length , _pixel color );
		
		/**
		 * Draw a Rectangle onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start (top)
		 * @param width Width of the Rectangle (1 means: 1Pixel in width)
		 * @param height Height of the Rectangle (1 means: 1Pixel in height)
		 * @param color Color of the Rect
		 * @return void
		**/
		void drawRect( _coord x , _coord y , _length width , _length height , _pixel color );
		
		/**
		 * Draw a filled Rectangle onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start (top)
		 * @param width Width of the Rectangle (1 means: 1Pixel in width)
		 * @param height Height of the Rectangle (1 means: 1Pixel in height)
		 * @param color Color of the Rect
		 * @return void
		**/
		void drawFilledRect( _coord x , _coord y , _length width , _length height , _pixel color );
		
		/**
		 * Draw a Gradient onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start (top)
		 * @param width Width of the Rectangle (1 means: 1Pixel in width)
		 * @param height Height of the Rectangle (1 means: 1Pixel in height)
		 * @param fromColor Start Color (top)
		 * @param toColor End Color (bottom)
		 * @return void
		**/
		void drawVerticalGradient( _coord x , _coord y , _length width , _length height , _pixel fromColor , _pixel toColor );
		
		/**
		 * Draw a Gradient onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start (top)
		 * @param width Width of the Rectangle (1 means: 1Pixel in width)
		 * @param height Height of the Rectangle (1 means: 1Pixel in height)
		 * @param fromColor Start Color (top)
		 * @param toColor End Color (bottom)
		 * @return void
		**/
		void drawHorizontalGradient( _coord x , _coord y , _length width , _length height , _pixel fromColor , _pixel toColor );
		
		/**
		 * Draw a Circle onto the bmp
		 * @param xc X-Position Center
		 * @param yc Y-Position Center
		 * @param radius Radius of the Circle
		 * @param color Color of the Circle
		 * @return void
		**/
		void drawCircle( _coord xc, _coord yc, _length radius, _pixel color);
		
		/**
		 * Draw a filled Circle onto the bmp
		 * @param xc X-Position Center
		 * @param yc Y-Position Center
		 * @param radius Radius of the Circle
		 * @param color Color of the Circle
		 * @return void
		**/
		void drawFilledCircle( _coord xc, _coord yc, _length radius, _pixel color);
		
		/**
		 * Draw an Ellipse onto the bmp
		 * @param xc X-Position Center
		 * @param yc Y-Position Center
		 * @param a Semi-Major Axis (in pixels)(=>Gro�e Bahnhalbachse)
		 * @param b Semi-Minor Axis (in pixels)(=>Kleine Bahnhalbachse)
		 * @param color Color of the Ellipse
		 * @return void
		 * @see 99392 ticks on dimensions 256*192
		**/
		void drawFilledEllipse( _coord xc, _coord yc, _length a, _length b, _pixel color);
		
		/**
		 * Draw an Ellipse onto the bmp
		 * @param xc X-Position Center
		 * @param yc Y-Position Center
		 * @param a Semi-Major Axis (in pixels)(=>Gro�e Bahnhalbachse)
		 * @param b Semi-Minor Axis (in pixels)(=>Kleine Bahnhalbachse)
		 * @param color Color of the Ellipse
		 * @return void
		**/
		void drawEllipse( _coord xc, _coord yc, _length a, _length b, _pixel color);
		
		/**
		 * Draw a single Character to a specific Position and returns the width it used
		 * @param x0 X-Position Left
		 * @param y0 Y-Position Top
		 * @param font The _font to use
		 * @param ch Character to draw (ASCII)
		 * @param color Color of the Character
		 * @return int The Width of the Character it has drawn
		**/
		_u16 drawChar( _coord x0 , _coord y0 , _font* font , _char ch , _pixel color , _u8 fontSize = 0 );
		
		/**
		 * Draw a String to a specific Position
		 * @param x0 X-Position Left
		 * @param y0 Y-Position Top
		 * @param font The _font to use
		 * @param str The String to draw
		 * @param color Color of the String
		 * @return void
		**/
		void drawString( _coord x0 , _coord y0 , _font* font , string str , _pixel color , _u8 fontSize = 0 );
		
		/**
		 * Copy a _bitmap onto the bitmap
		 * @param x 		X-Position Left
		 * @param y 		Y-Position Top
		 * @param data 		Other _bitmap
		 * @param bitmapX 	X-Origin of copying on the _bitmap (optional)
		 * @param bitmapY 	Y-Origin of copying on the _bitmap (optional)
		 * @return void
		**/
		void copy( _coord x , _coord y , const _bitmap* data );
		
		/**
		 * Copy a _bitmap onto the bitmap but with respecting the alpha channel
		 * @param x X-Position Left
		 * @param y Y-Position Top
		 * @param data Other _bitmap
		 * @return void
		**/
		void copyTransparent( _coord x , _coord y , const _bitmap* data , _pixel transparentColor = NO_COLOR );
		
		/**
		 * Copy a _bitmap onto the bitmap by taking the line x=0 and stretching that line over width
		 * @param x X-Position Left
		 * @param y Y-Position Top
		 * @param width Width to stretch it to
		 * @return void
		**/
		void copyHorizontalStretch( _coord x , _coord y , _length width , const _bitmap* data );
		
		/**
		 * Copy a _bitmap onto the bitmap by taking the row y=0 and stretching that line over width
		 * @param x X-Position Left
		 * @param y Y-Position Top
		 * @param height Height to stretch it to
		 * @return void
		**/
		void copyVerticalStretch( _coord x , _coord y , _length height , const _bitmap* data );
		
		/**
		 * Set the active ClippingRect
		 * @param rc Rect The rect to be clipped to
		 * @return void
		**/
		void setClippingRect( _rect rc );
		
		/**
		 * Get the active ClippingRect
		 * @return _rect
		**/
		_rect getClippingRect();
		
		/**
		 * Make the _bitmap not checking Coordinates
		 * @return void
		**/
		void resetClippingRect();
		
		/**
		 * Check if a Rectangle specified by borders is visible if it gets clipped by the activeClippingRect
		 * @param s16 left Left side of the Rectangle to check
		 * @param s16 top Top side of the Rectangle to check
		 * @param s16 right Right side of the Rectangle to check
		 * @param s16 bottom Bottom side of the Rectangle to check
		 * @return bool Whether it is visible (true) or not (false)
		**/
		private:
		bool clipCoordinates( _coord &left , _coord &top , _coord &right , _coord &bottom );
		public:
};

#endif