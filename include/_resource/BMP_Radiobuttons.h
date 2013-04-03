#ifndef _BMP_CHECKBOX_H_
#define _BMP_CHECKBOX_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

//
// Checked
//
static const _pixel BMP_RadioChecked_bmp[25] = {
63487, 63454, 63421, 64511, 65535, 
63422, 63421, 63454, 64478, 65535, 
63421, 63454, 64478, 64478, 65535, 
65535, 64478, 64478, 65535, 65535, 
65535, 65535, 65535, 65535, 65535, 
};

class BMP_RadioChecked : public _constbitmap {
	public:
		BMP_RadioChecked() : _constbitmap( const_cast<_pixelArray>(BMP_RadioChecked_bmp), 5, 5 ) { }
};

//
// Unchecked
//
static const _pixel BMP_RadioUnchecked_bmp[25] = {
64511, 64479, 60315, 65535, 65535, 
64511, 53140, 39654, 49969, 65535, 
60315, 38662, 35523, 33312, 61371, 
64511, 46862, 33312, 44747, 65535, 
65535, 65535, 58232, 65535, 65535, 
};

class BMP_RadioUnchecked : public _constbitmap {
	public:
		BMP_RadioUnchecked() : _constbitmap( const_cast<_pixelArray>(BMP_RadioUnchecked_bmp), 5, 5 ) { }
};

//
// Border-Pressed
//
static const _pixel BMP_RadioBorderPressed_bmp[81] = {
0    , 0    , 54895, 51657, 51656, 51657, 54863, 0    , 0    , 
0    , 53772, 53771, 62331, 0    , 63388, 53772, 53772, 0    , 
54896, 52747, 0    , 0    , 0    , 0    , 0    , 54828, 54896, 
51657, 61274, 0    , 0    , 0    , 0    , 0    , 64444, 51657, 
51657, 0    , 0    , 0    , 0    , 0    , 0    , 0    , 52714, 
51657, 64445, 0    , 0    , 0    , 0    , 0    , 65535, 52681, 
53838, 54862, 0    , 0    , 0    , 0    , 0    , 56943, 53806, 
0    , 52714, 54828, 63420, 0    , 64444, 54861, 52714, 0    , 
0    , 0    , 54863, 51657, 52713, 52681, 53839, 0    , 0    
};

class BMP_RadioBorderPressed : public _constbitmap {
	public:
		BMP_RadioBorderPressed() : _constbitmap( const_cast<_pixelArray>(BMP_RadioBorderPressed_bmp), 9, 9 ) { }
};

//
// Border
//
static const _pixel BMP_RadioBorder_bmp[81] = {
0    , 0    , 57010, 53770, 54826, 54795, 57010, 0    , 0    , 
0    , 55918, 55885, 63388, 0    , 63420, 55918, 55918, 0    , 
57011, 55885, 0    , 0    , 0    , 0    , 0    , 56942, 57010, 
53771, 62331, 0    , 0    , 0    , 0    , 0    , 64445, 54827, 
54826, 0    , 0    , 0    , 0    , 0    , 0    , 0    , 54860, 
54827, 64446, 0    , 0    , 0    , 0    , 0    , 65535, 54827, 
55953, 56976, 0    , 0    , 0    , 0    , 0    , 58033, 55953, 
0    , 55884, 56942, 64445, 0    , 64445, 56975, 55884, 0    , 
0    , 0    , 57010, 54827, 54859, 54827, 57010, 0    , 0    
};

class BMP_RadioBorder : public _constbitmap {
	public:
		BMP_RadioBorder() : _constbitmap( const_cast<_pixelArray>(BMP_RadioBorder_bmp), 9, 9 ) { }
};

#endif