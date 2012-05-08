#ifndef _WIN_F_PNG_
#define _WIN_F_PNG_

#include "_type/type.h"
#include "_type/type.bitmap.h"
#include "_type/type.direntry.h"
#include "_file/png.h"


class _png : public _bitmap , private _direntry 
{
	
	private:
	
		YsRawPngDecoder decoder;
	
	public:
	
		_png( string fn );
		
		~_png();
		
};
#endif
