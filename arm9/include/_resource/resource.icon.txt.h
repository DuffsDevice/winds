#ifndef _BMP_TXTICON_H_
#define _BMP_TXTICON_H_

#include <_type/type.h>
#include <_type/type.bitmap.h>

static const _pixel BMP_TxtIcon_bmp[80] = {
49680, 49680, 49680, 49680, 49680, 49680, 0	   , 0	  , 49680, 65535, 65535, 
65535, 65535, 49680, 49680, 0	 , 49680, 65535, 65535, 65535, 65535, 49680, 
49680, 49680, 49680, 65535, 58136, 58136, 58136, 65535, 65535, 49680, 49680, 
65535, 65535, 65535, 65535, 65535, 65535, 49680, 49680, 65535, 58136, 58136, 
58136, 58136, 65535, 49680, 49680, 65535, 65535, 65535, 65535, 65535, 65535, 
49680, 49680, 65535, 58136, 58136, 58136, 58136, 65535, 49680, 49680, 65535, 
65535, 65535, 65535, 65535, 65535, 49680, 49680, 49680, 49680, 49680, 49680, 
49680, 49680, 49680
};

class BMP_TxtIcon : public _constBitmap {
	public:
		BMP_TxtIcon() : _constBitmap(const_cast<_pixelArray>(BMP_TxtIcon_bmp), 8, 10) { }
};

#endif