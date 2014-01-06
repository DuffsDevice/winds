#ifndef _WIN_F_IMAGEFILE_
#define _WIN_F_IMAGEFILE_

#include "_type/type.h"
#include "_type/type.bitmap.h"
#include "_type/type.direntry.h"

enum class _imageFileCompression : _u8{
	low = 1,
	middle = 2,
	high = 3,
	veryHigh = 4,
};

class _imageFile : public _direntry
{
	public:
		
		//! Ctor
		_imageFile( string filename ) :
			_direntry( filename )
		{}
		
		//! Get the underlying bitmap Format
		_bitmap readBitmap();
		
		//! Write to the image
		void writeBitmap( _optValue<_imageFileCompression> compression );
};
#endif
