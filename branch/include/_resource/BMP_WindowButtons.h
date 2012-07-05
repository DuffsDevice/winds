#define _BMP_WINDOWBUTTONS_H_
#ifndef _BMP_WINDOWBUTTONS_H_
#define _BMP_WINDOWBUTTONS_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_WindowButtonMinimize_bmp[64] = {
0	 , 63977, 63977, 63977, 63977, 63977, 63977, 0	  , 
63977, 65235, 65134, 64044, 64044, 64010, 63944, 63977, 
63977, 65134, 64011, 63944, 63911, 63909, 63909, 63977, 
63977, 64043, 64010, 63976, 63942, 63908, 63908, 63977, 
63977, 64010, 64010, 63976, 63974, 63940, 63907, 63977, 
63977, 63976, 65535, 65535, 65535, 64963, 63906, 63977, 
63977, 63877, 63910, 63909, 63940, 63906, 63872, 63977, 
0	 , 63977, 63977, 63977, 63977, 63977, 63977, 0	  
};

class BMP_WindowButtonMinimize : public _bitmap {
	public:
		BMP_WindowButtonMinimize() : _bitmap(const_cast<_pixelArray>(BMP_WindowButtonMinimize_bmp), 8, 8) { }
};

static const _pixel BMP_WindowButtonMaximize_bmp[64] = {
0	 , 63977, 63977, 63977, 63977, 63977, 63977, 0	  , 
63977, 65201, 65134, 64044, 64044, 64010, 63944, 63977, 
63977, 65101, 65535, 65535, 65535, 65535, 63909, 63977, 
63977, 64043, 65267, 63976, 63942, 65267, 63908, 63977, 
63977, 64010, 65267, 63976, 63974, 65267, 63907, 63977, 
63977, 63977, 65267, 65267, 65267, 65267, 63906, 63977, 
63977, 63909, 63910, 63909, 63940, 63906, 63872, 63977, 
0	 , 63977, 63977, 63977, 63977, 63977, 63977, 0	  
};

class BMP_WindowButtonMaximize : public _bitmap {
	public:
		BMP_WindowButtonMaximize() : _bitmap(const_cast<_pixelArray>(BMP_WindowButtonMaximize_bmp), 8, 8) { }
};

static const _pixel BMP_WindowButtonClose_bmp[64] = {
0	 , 43484, 42396, 40316, 40316, 39260, 36123, 0	  , 
45565, 50813, 47677, 45565, 44509, 43452, 39292, 35029, 
42428, 46621, 65535, 41372, 39260, 65535, 38235, 36085, 
40316, 44509, 43452, 65535, 65535, 37180, 37180, 36053, 
39260, 43484, 43452, 65535, 65535, 37212, 36156, 35030, 
36123, 42396, 65535, 41372, 38269, 65535, 34076, 32950, 
34011, 38235, 39260, 38236, 36188, 35132, 33020, 32917, 
0	 , 35029, 35029, 35029, 33974, 32950, 32917, 0		
};

class BMP_WindowButtonClose : public _bitmap {
	public:
		BMP_WindowButtonClose() : _bitmap(const_cast<_pixelArray>(BMP_WindowButtonClose_bmp), 8, 8) { }
};

#endif