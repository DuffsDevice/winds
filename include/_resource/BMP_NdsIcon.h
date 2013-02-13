#ifndef _BMP_NDSICON_H_
#define _BMP_NDSICON_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_NdsIcon_bmp[54] = {
58136, 32768, 32768, 32768, 32768, 58136, 
32768, 65535, 65535, 65535, 65535, 32768, 
32768, 65535, 65535, 65535, 65535, 32768, 
58136, 32768, 32768, 32768, 32768, 58136, 
0    , 0    , 0    , 0    , 0    , 0    , 
59193, 48623, 48623, 48623, 48623, 59193, 
48623, 65535, 65535, 65535, 65535, 48623, 
48623, 65535, 65535, 65535, 65535, 48623, 
59193, 48623, 48623, 48623, 48623, 59193
};

class BMP_NdsIcon : public _bitmap {
	public:
		BMP_NdsIcon() : _bitmap(const_cast<_pixelArray>(BMP_NdsIcon_bmp), 6 , 9 ) { }
};

#endif