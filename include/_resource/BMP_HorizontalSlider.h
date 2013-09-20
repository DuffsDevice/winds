#ifndef _BMP_HORIZ_SLIDER_H_
#define _BMP_HORIZ_SLIDER_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_HorizontalSlider_bmp[66] = {
0    , 53009, 53042, 53042, 49902, 0    , 
55091, 39719, 36612, 37637, 33505, 44617, 
61305, 61404, 59322, 60346, 58265, 55988, 
61305, 65535, 65535, 65535, 65503, 55988, 
61305, 64511, 63454, 64511, 62397, 55988, 
61305, 64511, 63454, 64511, 62397, 55988, 
61305, 65535, 63454, 64511, 64479, 55988, 
54034, 58329, 65535, 65535, 56183, 46635, 
55027, 42663, 60379, 58233, 38339, 51857, 
0    , 56052, 43656, 39397, 51825, 0    , 
0    , 0    , 54995, 52914, 0    , 0    
};

class BMP_HorizontalSlider : public _constbitmap {
	public:
		BMP_HorizontalSlider() : _constbitmap(const_cast<_pixelArray>(BMP_HorizontalSlider_bmp), 6, 11) { }
};

#endif