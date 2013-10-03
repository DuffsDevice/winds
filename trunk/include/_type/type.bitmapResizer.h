// Check if already included
#ifndef _WIN_T_BITMAPRESIZER_
#define _WIN_T_BITMAPRESIZER_

#include "_type/type.bitmap.h"

class _bitmapResizer : public _bitmap
{
	
	private:
		
		void scaleRect( _bitmap& dest , _constbitmap& src, _rect srcArea = _rect() );
		
		_constbitmap*	src;
		_rect	 		srcArea;
		
	public:
		
		_bitmapResizer( _length width , _length height , _constbitmap& src , _rect srcArea = _rect() , bool doNotCompute = false );
		
		// Does the computing
		void compute();
		
		void setSrcRect( _rect area );
		
};

#endif