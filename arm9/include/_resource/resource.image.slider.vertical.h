#ifndef _BMP_VERTICAL_SLIDER_H_
#define _BMP_VERTICAL_SLIDER_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_VerticalSlider_bmp[72] = {
0    , 57173, 62363, 62330, 62330, 62330, 62330, 62330, 63387, 59256, 52976, 0    , 
50959, 50065, 65535, 64511, 64511, 64511, 64511, 64511, 65535, 57305, 35458, 49773, 
49967, 47983, 65535, 63454, 63454, 63454, 63454, 63454, 65535, 57272, 36451, 49774, 
50992, 49040, 65535, 64511, 64511, 64511, 64511, 64511, 65535, 58297, 36451, 49806, 
47852, 44908, 65503, 62397, 62397, 62397, 62397, 62397, 65503, 56215, 35394, 48717, 
0    , 47691, 55956, 54931, 54931, 54931, 54931, 54931, 55956, 51856, 47660, 0      
};

class BMP_VerticalSlider : public _constBitmap {
	public:
		BMP_VerticalSlider() : _constBitmap(const_cast<_pixelArray>(BMP_VerticalSlider_bmp), 12, 6) { }
};

#endif