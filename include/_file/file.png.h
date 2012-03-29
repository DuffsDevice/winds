#ifndef _WIN_F_PNG__
#define _WIN_F_PNG_

#include "_type/type.h"
#include "_type/type.bitmap.h"
#include "_type/type.file.h"
#include "_file/png.h"


class _png : private _file , public _bitmap {
	
	private:
		
		YsRawPngDecoder decoder;
		
	public:
		
		_png( string fn );
		
		void decode();
		
		_pixelArray getBitmap();
		
		_pixelArray getBitmap( _length y );
		
		_pixelArray getBitmap( _length x , _length y );
		
};
#endif
