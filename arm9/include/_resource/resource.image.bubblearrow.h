#ifndef _BMP_BUBBLEARROW_H_
#define _BMP_FONTICON_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_BubbleArrow_bmp[36] = {
40167, 62463, 62463, 62463, 62463, 40167, 
0    , 40167, 62463, 62463, 62463, 40167, 
0    , 0    , 40167, 62463, 62463, 40167, 
0    , 0    , 0    , 40167, 62463, 40167, 
0    , 0    , 0    , 0    , 40167, 40167, 
0    , 0    , 0    , 0    , 0    , 40167
};

class BMP_BubbleArrow : public _constBitmap {
	public:
		BMP_BubbleArrow() : _constBitmap(const_cast<_pixelArray>(BMP_BubbleArrow_bmp), 6 , 6 ) { }
};

#endif