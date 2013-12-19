// Check if already included
#ifndef _WIN_T_BITMAPPORT_
#define _WIN_T_BITMAPPORT_

#include "_type/type.h"
#include "_type/type.bitmap.h"
#include "_type/type.rect.h"

class _bitmapPort
{	
	private:
		
		friend class _gadget;
		
		//! Base Bitmap
		_bitmap& base;
		
		//! A list of Clipping-Rects
		_area clippingRects;
		
	public:
		
		/**
		 * Construcor
		 * @param bm the base _bitmap the work is done on
		 */
		_bitmapPort( _bitmap& bm ) :
			base( bm )
		{}
		
		/**
		 * Construcor
		 * @param bm the base _bitmap the work is done on
		 * @param clippings clippingRects to work in
		 */
		_bitmapPort( _bitmap& bm , _area clippings ) :
			base( bm )
			, clippingRects( (_area&&)clippings )
		{
			this->clippingRects.clipToIntersect( _rect( 0 , 0 , this->getWidth() , this->getHeight() ) );
		}
		
		/**
		 * Add a Clipping Rect to the list
		 * @param cR the Clipping-Rect to add
		 */
		void addClippingRect( _rect cR ){
			this->clippingRects.add( cR.clipToIntersect( _rect( 0 , 0 , this->getWidth() , this->getHeight() ) ) );
		}
		
		/**
		 * Add a couple of Clipping Rects to the list
		 * @param cR the Clipping-Rect to add
		 */
		void addClippingRects( _area cRs ){
			this->clippingRects.add( cRs.clipToIntersect( _rect( 0 , 0 , this->getWidth() , this->getHeight() ) ) );
		}
		
		/**
		 * Add a Clipping-Rect, that is full size
		 */
		void normalizeClippingRects(){
			this->deleteClippingRects();
			this->clippingRects.add( _rect( 0 , 0 , this->base.getWidth() , this->base.getHeight() ) );
		}
		
		/**
		 * Delete all Clipping Rects from the list
		 */
		void deleteClippingRects(){
			clippingRects.clearRects();
		}
		
		/**
		 * Operator for [i] to get a specific position of the bmp
		 */
		_pixel& operator[]( const _u32 pos ){
			return this->base[pos];
		}
		
		/**
		 * Operator for [x][y] to get a specific pixel (x,y) of the bmp
		 */
		_pixel& operator()( const _coord x , const _coord y ){
			return this->base( x , y );
		}
		
		/**
		 * Get the Bitmap's Width
		 * @return u16 Width of the _bitmap
		 */
		_length getWidth() const {
			return this->base.getWidth();
		}
		
		/**
		 * Get the Bitmap's Height
		 * @return u16 Height of the _bitmap
		 */
		_length getHeight() const {
			return this->base.getHeight();
		}
		
		/**
		 * Get the Pixel at a specific location
		 * @param x X-Position to check
		 * @param y Y-Position to check
		 * @return _pixel The Pixel at the specified location (if not foound: NO_COLOR)
		 */
		_pixel getPixel( const _coord x , const _coord y ) const {
			return this->base.getPixel( x , y );
		}
		
		/**
		 * Set the Pixel at a specific location
		 * @param x X-Position
		 * @param y Y-Position
		 * @param color Color of the Pixel to set
		 * @return void
		 */
		void drawPixel( _coord x , _coord y , _pixel color );
		
		/**
		 * Fill the whole bmp
		 * @param color The Color to fill with
		 * @return void
		 */
		void fill( _pixel color );
		
		/**
		 * Replace all pixels of a certain color with another
		 * @param color The Color to replace
		 * @param replace The Color to replace with
		 * @return void
		 */
		void replaceColor( _pixel color , _pixel replace );
		
		/**
		 * Draw a vertical Line onto the bmp
		 * @param x X-Position
		 * @param y Y-Position Start (top)
		 * @param length Length of the Line (height)
		 * @param color Color of the Line
		 * @return void
		 */
		void drawVerticalLine( _coord x , _coord y , _length length , _pixel color );
		
		/**
		 * Draw a horizontal Line onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start
		 * @param length Length of the Line (width)
		 * @param color Color of the Line
		 * @return void
		 */
		void drawHorizontalLine( _coord x , _coord y , _length length , _pixel color );
		
		/**
		 * Draw a vertical dotted Line onto the bmp
		 * @param x X-Position
		 * @param y Y-Position Start (top)
		 * @param length Length of the Line (height)
		 * @param color Color of the Line
		 * @return void
		 */
		void drawVerticalDottedLine( _coord x , _coord y , _length length , _pixel color );
		
		/**
		 * Draw a horizontal dotted Line onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start
		 * @param length Length of the Line (width)
		 * @param color Color of the Line
		 * @return void
		 */
		void drawHorizontalDottedLine( _coord x , _coord y , _length length , _pixel color );
		
		/**
		 * Draw a horizontal Line onto the bmp
		 * @param x1 Start-X-Position
		 * @param y1 Start-Y-Position
		 * @param x2 End-X-Position
		 * @param y2 End-Y-Position
		 * @param color Color of the Line
		 * @return void
		 */
		void drawLine( _coord x1 , _coord y1 , _coord x2 , _coord y2 , _pixel color );
		
		/**
		 * Draw a Rectangle onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start (top)
		 * @param width Width of the Rectangle (1 means: 1Pixel in width)
		 * @param height Height of the Rectangle (1 means: 1Pixel in height)
		 * @param color Color of the Rect
		 * @return void
		 */
		void drawRect( _coord x , _coord y , _length width , _length height , _pixel color );
		
		/**
		 * Draw a dotted Rectangle onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start (top)
		 * @param width Width of the Rectangle (1 means: 1Pixel in width)
		 * @param height Height of the Rectangle (1 means: 1Pixel in height)
		 * @param color Color of the Rect
		 * @return void
		 */
		void drawDottedRect( _coord x , _coord y , _length width , _length height , _pixel color );
		
		/**
		 * Draw a filled Rectangle onto the bmp
		 * @param x X-Position (left)
		 * @param y Y-Position Start (top)
		 * @param width Width of the Rectangle (1 means: 1Pixel in width)
		 * @param height Height of the Rectangle (1 means: 1Pixel in height)
		 * @param color Color of the Rect
		 * @return void
		 */
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
		 */
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
		 */
		void drawHorizontalGradient( _coord x , _coord y , _length width , _length height , _pixel fromColor , _pixel toColor );
		
		/**
		 * Draw a Circle onto the bmp
		 * @param xc X-Position Center
		 * @param yc Y-Position Center
		 * @param radius Radius of the Circle
		 * @param color Color of the Circle
		 * @return void
		 */
		void drawCircle( _coord xc, _coord yc, _length radius, _pixel color );
		
		/**
		 * Draw a filled Circle onto the bmp
		 * @param xc X-Position Center
		 * @param yc Y-Position Center
		 * @param radius Radius of the Circle
		 * @param color Color of the Circle
		 * @return void
		 */
		void drawFilledCircle( _coord xc, _coord yc, _length radius, _pixel color );
		
		/**
		 * Draw a filled Ellipse onto the bmp
		 * @param xc X-Position Center
		 * @param yc Y-Position Center
		 * @param a Semi-Major Axis (in pixels)(=>Große Bahnhalbachse)
		 * @param b Semi-Minor Axis (in pixels)(=>Kleine Bahnhalbachse)
		 * @param color Color of the Ellipse
		 * @return void
		 * @see 99392 ticks on dimensions 256*192
		 */
		void drawFilledEllipse( _coord xc, _coord yc, _length a, _length b, _pixel color );
		
		/**
		 * Draw an Ellipse onto the bmp
		 * @param xc X-Position Center
		 * @param yc Y-Position Center
		 * @param a Semi-Major Axis (in pixels)(=>Große Bahnhalbachse)
		 * @param b Semi-Minor Axis (in pixels)(=>Kleine Bahnhalbachse)
		 * @param color Color of the Ellipse
		 * @return void
		 */
		void drawEllipse( _coord xc, _coord yc, _length a, _length b, _pixel color );
		
		/**
		 * Draw a single _character to a specific Position and returns the width it used
		 * @param x0 X-Position Left
		 * @param y0 Y-Position Top
		 * @param font The _font to use
		 * @param ch _character to draw (ASCII)
		 * @param color Color of the _character
		 * @return int The Width of the _character it has drawn
		 */
		_length drawChar( _coord x0 , _coord y0 , const _font* font , _char ch , _pixel color , _u8 fontSize = 0 );
		
		/**
		 * Draw a String to a specific Position
		 * @param x0 X-Position Left
		 * @param y0 Y-Position Top
		 * @param font The _font to use
		 * @param str The String to draw
		 * @param color Color of the String
		 * @return void
		 */
		void drawString( _coord x0 , _coord y0 , const _font* font , const _char* str , _pixel color , _u8 fontSize = 0 );
		void drawString( _coord x0 , _coord y0 , const _font* font , string str , _pixel color , _u8 fontSize = 0 )
		{
			drawString( x0 , y0 , font , str.c_str() , color , fontSize );
		}
		
		/**
		 * Copy a _bitmap onto the bitmap
		 * @param x 		X-Position Left
		 * @param y 		Y-Position Top
		 * @param data 		Other _bitmap
		 * @return void
		 */
		void copy( _coord x , _coord y , _constbitmap& data );
		
		/**
		 * Copy a _bitmap onto the bitmap but with respecting the alpha channel
		 * @param x X-Position Left
		 * @param y Y-Position Top
		 * @param data Other _bitmap
		 * @return void
		 */
		void copyTransparent( _coord x , _coord y , _constbitmap& data );
		
		/**
		 * Copy a _bitmap onto the bitmap by taking the line x=0 and stretching that line over width
		 * @param x X-Position Left
		 * @param y Y-Position Top
		 * @param width Width to stretch it to
		 * @param data Other _bitmap to copy
		 * @return void
		 */
		void copyHorizontalStretch( _coord x , _coord y , _length width , _constbitmap& data );
		
		/**
		 * Copy a _bitmap onto the bitmap by taking the row y=0 and stretching that line over width
		 * @param x X-Position Left
		 * @param y Y-Position Top
		 * @param width Width to stretch it to
		 * @param data Other _bitmap to copy
		 * @return void
		 */
		void copyVerticalStretch( _coord x , _coord y , _length width , _constbitmap& data );
};

#endif