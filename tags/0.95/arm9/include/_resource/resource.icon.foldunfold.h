#ifndef _BMP_FOLDUNFOLDICON_H_
#define _BMP_FOLDUNFOLDICON_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_FoldUnFoldIcon_bmp[49] = {
55919, 55919, 56943, 56943, 56943, 55919, 55919, 
55919, 65535, 65535, 65535, 65535, 65535, 55919, 
55919, 65535, 64478, 64478, 64478, 62398, 54862, 
55919, 64511, 64478, 64478, 62397, 59260, 54862, 
55887, 62398, 61340, 60284, 60283, 58171, 54862, 
54830, 56122, 53976, 52952, 52951, 52952, 53805, 
55919, 53805, 52749, 52749, 52749, 52749, 55919
};

class BMP_FoldUnFoldIcon : public _constBitmap {
	public:
		BMP_FoldUnFoldIcon() : _constBitmap(const_cast<_pixelArray>(BMP_FoldUnFoldIcon_bmp), 7 , 7 ) { }
};

#endif