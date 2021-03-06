#ifndef _BMP_MUSICICON_H_
#define _BMP_MUSICICON_H_

#include <_type/type.h>
#include <_type/type.bitmap.h>

static const _pixel BMP_MusicIcon_bmp[90] = {
49680, 49680, 49680, 49680, 49680, 49680, 0    , 0    , 
49680, 65535, 65535, 65535, 65535, 49680, 49680, 0    , 
49680, 65535, 65535, 65535, 65535, 65535, 49680, 49680, 
49680, 65535, 64511, 65535, 58663, 63347, 65535, 49680, 
49680, 65535, 65535, 65535, 58663, 56910, 63347, 49680, 
49680, 65535, 65535, 65535, 58663, 65535, 63347, 49680, 
49680, 65535, 63347, 58663, 58663, 65535, 65535, 49680, 
49680, 65535, 58663, 58663, 63347, 65535, 65535, 49680, 
49680, 65535, 65535, 65535, 65535, 65535, 65535, 49680, 
49680, 49680, 49680, 49680, 49680, 49680, 49680, 49680
};

class BMP_MusicIcon : public _constBitmap {
	public:
		BMP_MusicIcon() : _constBitmap(const_cast<_pixelArray>(BMP_MusicIcon_bmp), 8 , 10 ) { }
};

#endif