#ifndef _BMP_TASKMGRICON_H_
#define _BMP_TASKMGRICON_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_TaskMgrIcon_bmp[90] = {
    0,     0, 52851, 51794, 52819, 52819, 52819, 52819, 52819,     0, 
58070, 52786, 33825, 33857, 35074, 44395, 45452, 44395, 42281, 56022, 
49874, 36455, 36900, 35201, 35489, 44523, 47534, 47534, 46413, 56022, 
55029, 53077, 36706, 34337, 34946, 36579, 46413, 43466, 35681, 57080, 
56022, 59161, 38085, 34882, 34818, 38149, 37636, 39557, 39269, 57080, 
56022, 58136, 33825, 32768, 32768, 33793, 36227, 36227, 34850, 56022, 
56022, 57079, 44395, 44395, 44395, 44395, 44395, 44395, 44395, 59193, 
57046, 52851,     0,     0,     0, 52851, 52851,     0,     0,     0, 
51761, 48623,     0,     0, 52851, 53908, 52851, 52851,     0,     0
};

class BMP_TaskMgrIcon : public _constBitmap {
	public:
		BMP_TaskMgrIcon() : _constBitmap(const_cast<_pixelArray>(BMP_TaskMgrIcon_bmp), 10 , 9 ) { }
};

#endif