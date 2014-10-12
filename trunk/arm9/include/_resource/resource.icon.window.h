#ifndef _BMP_WINDOW_H_
#define _BMP_WINDOW_H_

#include <_type/type.h>
#include <_type/type.bitmap.h>

static const _pixel BMP_WindowIcon_bmp[36] = {
60904, 60839, 62919, 51569, 46488, 55693, 
60805, 61929, 63909, 44409, 51967, 48398, 
58792, 59915, 60939, 52624, 46485, 55658, 
61206, 61374, 61373, 61373, 62429, 59125, 
61205, 61373, 61340, 61340, 61373, 59092, 
58991, 59092, 59060, 59060, 59092, 56878
};

class BMP_WindowIcon : public _constBitmap {
	public:
		BMP_WindowIcon() : _constBitmap(const_cast<_pixelArray>(BMP_WindowIcon_bmp), 6, 6) { }
};

#endif