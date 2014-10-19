#ifndef _BMP_checkBox_H_
#define _BMP_checkBox_H_

#include <_type/type.h>
#include <_type/type.bitmap.h>

//
// Checked
//
static const _pixel BMP_checkBoxChecked_bmp[49] = {
63455, 62397, 62398, 63454, 64479, 64511, 65535, 
62398, 63389, 62364, 63422, 62396, 44779, 64511, 
62397, 53043, 64479, 63421, 41640, 37508, 65535, 
62430, 37508, 50993, 43722, 36451, 56182, 65535, 
64479, 46829, 35426, 37476, 57239, 65535, 65535, 
64511, 64479, 46829, 55157, 65535, 65535, 65535, 
65535, 65535, 65535, 65535, 65535, 65535, 65535, 
};

class BMP_checkBoxChecked : public _constBitmap {
	public:
		BMP_checkBoxChecked() : _constBitmap( const_cast<_pixelArray>(BMP_checkBoxChecked_bmp), 7, 7 ) { }
};


//
// Semi-Checked
//
static const _pixel BMP_checkBoxSemi_bmp[49] = {
62431, 62397, 62397, 63422, 63454, 64479, 65535, 
62397, 41640, 41640, 41640, 41640, 41640, 65535, 
62397, 41640, 41640, 41640, 41640, 41640, 65535,
63422, 41640, 41640, 41640, 41640, 41640, 65535,
63454, 41640, 41640, 41640, 41640, 41640, 65535,
64479, 41640, 41640, 41640, 41640, 41640, 65535,
65535, 65535, 65535, 65535, 65535, 65535, 65535,

};

class BMP_checkBoxSemi : public _constBitmap {
	public:
		BMP_checkBoxSemi() : _constBitmap( const_cast<_pixelArray>(BMP_checkBoxSemi_bmp), 7, 7 ) { }
};


//
// Unchecked
//
static const _pixel BMP_checkBoxUnchecked_bmp[49] = {
62431, 62397, 62397, 63422, 63454, 64479, 65535,
62397, 61340, 62364, 62397, 63421, 63454, 65535,
62397, 62364, 62397, 63421, 63454, 64478, 65535,
63422, 62397, 63421, 63454, 64478, 64478, 65535,
63454, 63421, 63454, 64478, 64478, 64511, 65535,
64479, 63454, 64478, 64478, 64511, 65535, 65535,
65535, 65535, 65535, 65535, 65535, 65535, 65535,
};

class BMP_checkBoxUnchecked : public _constBitmap {
	public:
		BMP_checkBoxUnchecked() : _constBitmap( const_cast<_pixelArray>(BMP_checkBoxUnchecked_bmp), 7, 7 ) { }
};


#endif