#ifndef _BMP_SCROLLBUTTONS_H_
#define _BMP_SCROLLBUTTONS_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_ScrollButton_bmp[64] = { 
	0	 , 64345, 64311, 64311, 64310, 64310, 64278, 0    , 
	64345, 65402, 65401, 65369, 65368, 65368, 64311, 60015, 
	64311, 65369, 65368, 65368, 65367, 65335, 64278, 61072, 
	64311, 65368, 65368, 65368, 65367, 65367, 64277, 61072, 
	64310, 65368, 65367, 65367, 65367, 64343, 63253, 60049, 
	64278, 65367, 65367, 65367, 65367, 64311, 63253, 61104, 
	64277, 64311, 64311, 64311, 64310, 64277, 63220, 61105, 
	0    , 61073, 60015, 60015, 60047, 60048, 60048, 0
};

class BMP_ScrollButton : public _bitmap {
	public:
		BMP_ScrollButton() : _bitmap(const_cast<_pixelArray>(BMP_ScrollButton_bmp), 8, 8) { }
};

//
// Pressed
//

static const _pixel BMP_ScrollButtonPressed_bmp[64] = {
	0    , 61074, 61009, 61008, 60976, 59952, 60975, 0    , 
	61042, 63086, 63120, 63153, 63154, 63154, 63154, 64311, 
	61008, 63088, 63121, 63121, 63155, 63155, 63155, 64311, 
	60976, 63120, 62097, 63122, 63155, 63155, 63155, 64310, 
	60975, 62065, 63122, 63154, 63155, 63155, 63155, 64278, 
	60975, 63121, 63122, 63155, 63187, 63187, 63188, 64278, 
	60942, 63122, 63155, 63187, 63220, 63221, 63254, 63254, 
	0    , 63254, 64278, 64278, 64278, 64278, 64278, 0    
};

class BMP_ScrollButtonPressed : public _bitmap {
	public:
		BMP_ScrollButtonPressed() : _bitmap(const_cast<_pixelArray>(BMP_ScrollButtonPressed_bmp), 8, 8) { }
};

//
// Scroll Background
//

static const _pixel BMP_ScrollBg_bmp[8] = {
	62364, 63454, 63454, 64478, 64511, 64511, 65535, 63454
};

class BMP_ScrollBgSnipVertical : public _bitmap {
	public:
		BMP_ScrollBgSnipVertical() : _bitmap(const_cast<_pixelArray>(BMP_ScrollBg_bmp), 1, 8) { }
};

class BMP_ScrollBgSnipHorizontal : public _bitmap {
	public:
		BMP_ScrollBgSnipHorizontal() : _bitmap(const_cast<_pixelArray>(BMP_ScrollBg_bmp), 8, 1) { }
};


#endif