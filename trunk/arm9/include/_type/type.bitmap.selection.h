#ifndef _WIN_T_BITMAP_SELECTION_
#define _WIN_T_BITMAP_SELECTION_

#include <_type/type.h>
#include <_type/type.bitmap.h>
#include <_type/type.bitfield.h>

class _bitmapSelection
{
	private:
		
		_bitField	data;
		_length		width;
		_length		height;
		
		friend class _bitmapToner;
	
	public:
		
		//! Ctor
		_bitmapSelection( _length width , _length height , bool allSelected = false ) :
			data( width * height , allSelected )
			, width( width )
			, height( height )
		{}
		
		//! Get width of the selection
		_length getWidth() const { return this->width; }
		
		//! Get height of the selection
		_length getHeight() const { return this->height; }
		
		/**
		 * Creates a selection object that selects all pixels of the
		 * supplied bitmap that are within a color range
		 * @param origin The bitmap where to select from
		 * @param color The color to select
		 * @param tolerance The maximum distance of a pixel color to the selection color (in 3D-RGB-Space (0...54))
		 * @return void The selection object in the size of the bitmap having specific pixels selected
		 */
		static _bitmapSelection fromColorRange( const _bitmap& origin , _color color , _u8 tolerance = 0 );
		
		/**
		 * Creates a selection object that selects all pixels of the
		 * supplied bitmap that are within a brightness range
		 * @param origin The bitmap where to select from
		 * @param brightness The brightness value to select (0 (black) to 100 (white) )
		 * @param tolerance The maximum distance of a pixels brightness value to the specified brightness (0-100)
		 * @return void The selection object in the size of the bitmap having specific pixels selected
		 */
		static _bitmapSelection fromBrightnessRange( const _bitmap& origin , _u8 brightness , _u8 tolerance = 0 );
		
		//! Inverts the selection
		void invert(){
			this->data.flip();
		}
};

#endif