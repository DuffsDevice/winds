#ifndef _BMP_EXECUTORICON_H_
#define _BMP_EXECUTORICON_H_

#include <_type/type.h>
#include <_type/type.bitmap.h>

static const _pixel BMP_ExecutorIcon_bmp[81] = {
    0,     0,     0,     0, 32768,     0,     0,     0,     0, 
    0, 32768,     0,     0, 32768,     0,     0, 62364,     0, 
    0,     0, 32768,     0, 32768,     0, 62364,     0,     0, 
    0,     0,     0, 32768,     0, 62364,     0,     0,     0, 
48623, 48623, 48623,     0,     0,     0, 62364, 62364, 62364, 
    0,     0,     0, 48623,     0, 58136,     0,     0,     0, 
    0,     0, 48623,     0, 58136,     0, 58136,     0,     0, 
    0, 48623,     0,     0, 58136,     0,     0, 58136,     0, 
    0,     0,     0,     0, 58136,     0,     0,     0,     0
};

class BMP_ExecutorIcon : public _constBitmap {
	public:
		BMP_ExecutorIcon() : _constBitmap(const_cast<_pixelArray>(BMP_ExecutorIcon_bmp), 9, 9) { }
};

#endif