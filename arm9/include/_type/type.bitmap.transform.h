// Check if already included
#ifndef _WIN_T_BITMAP_TRANSFORM_
#define _WIN_T_BITMAP_TRANSFORM_

#include "_type/type.bitmap.h"

class _bitmapTransform : public _bitmap
{
	
	private:
		
		// Does the computing
		void scaleRect( _bitmap& dest , _constBitmap& src , _rect srcArea , bool flipH , bool flipV );
		
		// Necessary data
		_constBitmap*	src;
		_rect	 		srcArea;
		bool			flipHorizontal;
		bool			flipVertical;
		
	public:
		
		//! Ctor with scale and flip
		_bitmapTransform( _constBitmap& src , _length destWidth , _length destHeight , _rect srcArea = _rect() , bool doNotCompute = false , bool flipH = false , bool flipV = false );
		
		//! Ctor with flip
		_bitmapTransform( _constBitmap& src , bool flipH , bool flipV , bool doNotCompute = false ) :
			_bitmapTransform( src , src.getWidth() , src.getHeight() , _rect() , doNotCompute , flipH , flipV )
		{}
		
		//! Calls ScaleRect with the appropriate parameters
		void compute();
		
		//! Sets the source rect
		void setSrcRect( _rect area );
		
};

#endif