#ifndef _BMP_WINDOWHEADER_H_
#define _BMP_WINDOWHEADER_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_WindowHeader_bmp[10] = {
60870, 61926, 59748, 59716, 60772, 60772, 60772, 60804, 60772, 54531
};

class BMP_WindowHeader : public _bitmap {
	public:
		BMP_WindowHeader() : _bitmap(const_cast<_pixelArray>(BMP_WindowHeader_bmp), 1, 10) { }
};

#endif