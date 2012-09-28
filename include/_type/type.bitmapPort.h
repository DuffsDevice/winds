// Check if already included
#ifndef _WIN_T_BITMAPPORT_
#define _WIN_T_BITMAPPORT_

#include "type.bitmap.h"
#include "type.rect.h"
#include "type.h"

using namespace std;

class _bitmapPort{
	
	public:
		
		//! A list of Clipping-Rects
		_area clippingRects;
		
		//! Base Bitmap
		_bitmap* base;
		
	public:
		
		/**
		 * Construcor
		 * @param bm the base _bitmap the work is done on
		**/
		_bitmapPort( _bitmap* bm );
		
		/**
		 * Construcor
		 * @param bm the base _bitmap the work is done on
		 * @param clippings clippingRects to work in
		**/
		_bitmapPort( _bitmap* bm , _area clippings );
		
		/**
		 * Add a Clipping Rect to the list
		 * @param cR the Clipping-Rect to add
		**/
		void addClippingRects( _rect cR );
		
		/**
		 * Add a couple of Clipping Rects to the list
		 * @param cR the Clipping-Rect to add
		**/
		void addClippingRects( _area cR );
		
		/**
		 * Draw on the full bitmap with no rects
		**/
		void resetClippingRects();
		
		/**
		 * Delete all Clipping Rects from the list
		**/
		void deleteClippingRects();
		
		/**
		 * Operator for [i] to get a specific position of the bmp
		**/
		_pixel& operator[]( const _u32 pos );
		
		/**
		 * Operator for [x][y] to get a specific pixel (x,y) of the bmp
		**/
		_pixel& operator()( const _coord x , const _coord y );
		
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
		 * Get the Pixel at a specific location
		 * @param x X-Position to check
		 * @param y Y-Position to check
		 * @return _pixel The Pixel at the specified location (if not foound: NO_COLOR)
		**/
		_pixel getPixel( const _coord x , const _coord y ) const ;
		
		/**
		 * Set the Pixel at a specific location
		 * @param x X-Position
		 * @param y Y-Position
		 * @param color Color of the Pixel to set
		 * @return void
		**/
		void drawPixel( _coord x , _coord y , _pixel color );
		
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
		 * Draw a filled Ellipse onto the bmp
		 * @param xc X-Position Center
		 * @param yc Y-Position Center
		 * @param a Semi-Major Axis (in pixels)(=>Große Bahnhalbachse)
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
		 * @param a Semi-Major Axis (in pixels)(=>Große Bahnhalbachse)
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
		_length drawChar( _coord x0 , _coord y0 , _font* font , _char ch , _pixel color , _u8 fontSize = 0 );
		
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
		 * @return void
		**/
		void copy( _coord x , _coord y , const _bitmap* data );
		void copy( _coord x , _coord y , _bitmapPort data );
		
		/**
		 * Copy a _bitmap onto the bitmap but with respecting the alpha channel
		 * @param x X-Position Left
		 * @param y Y-Position Top
		 * @param data Other _bitmap
		 * @return void
		**/
		void copyTransparent( _coord x , _coord y , const _bitmap* data );
		void copyTransparent( _coord x , _coord y , _bitmapPort data );
		
		/**
		 * Copy a _bitmap onto the bitmap by taking the line x=0 and stretching that line over width
		 * @param x X-Position Left
		 * @param y Y-Position Top
		 * @param width Width to stretch it to
		 * @param data Other _bitmap to copy
		 * @return void
		**/
		void copyHorizontalStretch( _coord x , _coord y , _length width , const _bitmap* data );
		void copyHorizontalStretch( _coord x , _coord y , _length width , _bitmapPort data );
		
		/**
		 * Copy a _bitmap onto the bitmap by taking the row y=0 and stretching that line over width
		 * @param x X-Position Left
		 * @param y Y-Position Top
		 * @param width Width to stretch it to
		 * @param data Other _bitmap to copy
		 * @return void
		**/
		void copyVerticalStretch( _coord x , _coord y , _length width , const _bitmap* data );
		void copyVerticalStretch( _coord x , _coord y , _length width , _bitmapPort data );
};

#endif