#ifndef _BMP_WINDOWHEADER_H_
#define _BMP_WINDOWHEADER_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_WindowHeader_bmp[10] = {
60870, 61926, 59748, 59716, 60772, 60772, 60772, 60804, 60772, 54531
};

class BMP_WindowHeader : public _constBitmap {
	public:
		BMP_WindowHeader() : _constBitmap(const_cast<_pixelArray>(BMP_WindowHeader_bmp), 1, 10) { }
};

static const _pixel BMP_WindowHeaderBlurred_bmp[10] = {
57900, 57933, 56811, 56811, 56811, 56811, 56843, 57867, 56811, 52617
};

class BMP_WindowHeaderBlurred : public _constBitmap {
	public:
		BMP_WindowHeaderBlurred() : _constBitmap(const_cast<_pixelArray>(BMP_WindowHeaderBlurred_bmp), 1, 10) { }
};

#endif