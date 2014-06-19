#ifndef _WIN_T_BITMAP_TONER_
#define _WIN_T_BITMAP_TONER_

#include "_type/type.bitmap.h"
#include "_type/type.bitmap.selection.h"
#include "_type/type.color.palette.h"

class _bitmapToner
{
	private:
		
		_bitmap& base;
	
	public:
		
		//! Ctor
		_bitmapToner( _bitmap& base ) :
			base( base )
		{}
		
		//! Converts either the whole or a selection of the bitmap to greyscale
		//! @note the selection must have the same size as the bitmap
		void convertToGreyscale() const ;
		bool convertToGreyscale( const _bitmapSelection& selection ) const ;
		
		/**
		 * Each color gets converted to another in the supplied palette
		 * The index at which the color is taken from the palette
		 * is determined by the brightness value of the original pixel
		 * @param palette The palette to read from
		 * @param extendedRange Whether or not the brightness value spans 0..31 (false) or 0..100 (true)
		 * @note the selection must have the same size as the bitmap
		 */
		void convertBrightnessToPalette( _colorPalette palette , bool extendedRange = false ) const ;
		bool convertBrightnessToPalette( _colorPalette palette , const _bitmapSelection& selection , bool extendedRange = false ) const ;
		
		//! Replace all colors withing a selection with the specified color
		//! @note the selection must have the same size as the bitmap
		bool fill( const _bitmapSelection& selection , _color replacement ) const ;
		
		//! Inverts either the whole or a selection of the bitmap
		//! @note the selection must have the same size as the bitmap
		void invert() const ;
		bool invert( const _bitmapSelection& selection ) const ;
};

#endif