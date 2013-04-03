#ifndef _BMP_IMAGEICON_H_
#define _BMP_IMAGEICON_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_ImageIcon_bmp[80] = {
50533, 50533, 50533, 50533, 50533, 50533, 0    , 0    , 
50533, 64409, 64409, 64409, 64409, 54795, 50533, 0    , 
50533, 64409, 62262, 62262, 62262, 54795, 54795, 50533, 
50533, 64409, 35513, 35513, 35513, 35513, 65499, 50533, 
50533, 64409, 37595, 37627, 38684, 37627, 65499, 50533, 
50533, 64409, 39213, 38684, 47103, 38717, 65499, 50533, 
50533, 64409, 34324, 34325, 37789, 35447, 65499, 50533, 
50533, 64409, 33168, 33169, 34487, 33169, 65499, 50533, 
50533, 64409, 65499, 65499, 65499, 65499, 65499, 50533, 
50533, 50533, 50533, 50533, 50533, 50533, 50533, 50533
};

class BMP_ImageIcon : public _constbitmap {
	public:
		BMP_ImageIcon() : _constbitmap(const_cast<_pixelArray>(BMP_ImageIcon_bmp), 8 , 10 ) { }
};

#endif