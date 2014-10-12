#ifndef _BMP_WINDS_H_
#define _BMP_WINDS_H_

#include <_type/type.h>
#include <_type/type.bitmap.h>

static const _pixel BMP_WinDSIcon_bmp[90] = {
    0,     0,     0, 64302, 50858, 52021,     0,     0,     0, 
    0,     0, 65327, 56999, 43623, 47789, 53080,     0,     0, 
    0, 61137, 65254, 60106, 45671, 48849, 49941, 52022,     0, 
50746, 51630, 65351, 65321,     0, 50998, 53012, 41682, 47927, 
45396, 44340, 55858,     0,     0,     0, 46870, 38610, 39635, 
    0, 44306, 45501, 53951,     0, 54271, 43868, 36529, 53080, 
    0,     0, 46490, 48575, 46719, 44991, 42843, 52023,     0, 
    0,     0,     0, 46559, 38239, 44863,     0,     0,     0, 
    0,     0,     0,     0, 38303,     0,     0,     0,     0, 
    0,     0,     0,     0, 56127,     0,     0,     0,     0
};

class BMP_WinDSIcon : public _constBitmap {
	public:
		BMP_WinDSIcon() : _constBitmap(const_cast<_pixelArray>(BMP_WinDSIcon_bmp), 9, 10) { }
};

#endif