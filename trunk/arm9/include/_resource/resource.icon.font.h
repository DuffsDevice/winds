#ifndef _BMP_FONTICON_H_
#define _BMP_FONTICON_H_

#include <_type/type.h>
#include <_type/type.bitmap.h>

static const _pixel BMP_FontIcon_bmp[80] = {
49680, 49680, 49680, 49680, 49680, 49680, 0    , 0    , 
49680, 65535, 65535, 65535, 65535, 49680, 49680, 0    , 
49680, 65535, 65535, 60092, 63015, 65535, 49680, 49680, 
49680, 65535, 64511, 61112, 60716, 65528, 65535, 49680, 
49680, 65535, 63391, 64408, 58804, 65296, 65535, 49680, 
49680, 65535, 61215, 58663, 58663, 64172, 65535, 49680, 
49680, 65535, 63256, 65535, 62367, 61868, 65532, 49680, 
49680, 58804, 60711, 65528, 60092, 58663, 61863, 49680, 
49680, 65535, 65535, 65535, 65535, 65535, 65535, 49680, 
49680, 49680, 49680, 49680, 49680, 49680, 49680, 49680
};

class BMP_FontIcon : public _constBitmap {
	public:
		BMP_FontIcon() : _constBitmap(const_cast<_pixelArray>(BMP_FontIcon_bmp), 8 , 10 ) { }
};

#endif