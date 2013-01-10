#ifndef _WIN_F_IMAGEFILE_
#define _WIN_F_IMAGEFILE_

#include "_type/type.h"
#include "_type/type.bitmap.h"
#include "_type/type.direntry.h"
#include "_library/png.h"
#include "_library/jpeg.h"
#include "_library/bmp.h"


class _imagefile : public _bitmap , private _direntry 
{
	
	private:
	
		YsRawPngDecoder* pngDecoder;
		Jpeg::Decoder* jpgDecoder;
	
	public:
	
		_imagefile( string fn );
		
		~_imagefile();
		
};
#endif
