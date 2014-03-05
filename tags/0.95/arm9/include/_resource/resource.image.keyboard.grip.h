#ifndef _BMP_GRIP_H_
#define _BMP_GRIP_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_Grip_bmp[320] = {
0    , 0    , 0    , 41191, 43305, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 44362, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43305, 41191, 0    , 0    , 0    , 
0    , 0    , 0    , 41191, 45452, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46508, 46509, 46509, 46509, 46509, 45452, 41191, 0    , 0    , 0    , 
0    , 0    , 45452, 33825, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 33825, 45452, 0    , 0    , 
0    , 0    , 43305, 41224, 43305, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43305, 41224, 43305, 0    , 0    , 
0    , 0    , 36996, 45452, 45452, 45452, 46476, 46476, 45452, 45452, 46476, 46476, 46476, 46476, 46476, 46476, 46476, 46476, 46476, 46476, 46476, 45452, 45452, 45452, 46476, 46476, 45452, 45452, 45452, 36996, 0    , 0    , 
0    , 0    , 34882, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 34882, 0    , 0    , 
0    , 48623, 38053, 42281, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 43338, 42281, 38053, 48623, 0    , 
0    , 48590, 44362, 46476, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46509, 46476, 44362, 48590, 0    , 
48590, 35906, 33825, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 33792, 33792, 33792, 33792, 33792, 33792, 33792, 33792, 33792, 33792, 33792, 33792, 33792, 33792, 32768, 32768, 32768, 32768, 32768, 33825, 35906, 48590, 
};

class BMP_Grip : public _constBitmap {
	public:
		BMP_Grip() : _constBitmap(const_cast<_pixelArray>(BMP_Grip_bmp), 32, 9) { }
};

#endif