#ifndef _BMP_ACTIONBUTTONS_H_
#define _BMP_ACTIONBUTTONS_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_ActionBtnNext_bmp[81] = {
0	 , 43690, 42632, 40551, 40551, 39526, 38501, 36355, 0	 , 
43690, 52017, 47854, 45772, 45772, 43691, 41608, 38470, 35234, 
42633, 47854, 45772, 43658, 65535, 51057, 39526, 39494, 36259, 
40551, 45772, 44715, 42632, 40550, 65535, 47854, 38469, 36259, 
39493, 65535, 65535, 65535, 65535, 65535, 65535, 37476, 35234, 
37444, 42634, 43690, 42633, 40615, 65535, 47854, 36483, 34209, 
35362, 40583, 42633, 41608, 65535, 47854, 36546, 33408, 33184, 
34273, 37412, 39493, 38469, 37476, 36483, 34433, 33344, 33152, 
0	 , 34177, 35202, 35202, 34209, 34209, 33184, 33152, 0	
};

class BMP_ActionBtnNext : public _constBitmap {
	public:
		BMP_ActionBtnNext() : _constBitmap(const_cast<_pixelArray>(BMP_ActionBtnNext_bmp), 9, 9) { }
};

static const _pixel BMP_ActionBtnPrev_bmp[81] = {
0    , 43690, 42632, 40551, 40551, 39526, 38501, 36355, 0    , 
43690, 52017, 47854, 45772, 45772, 43691, 41608, 38470, 35234, 
42633, 47854, 45772, 52018, 65535, 41608, 39526, 39494, 36259, 
40551, 45772, 52018, 65535, 40583, 40583, 40583, 38469, 36259, 
39493, 45772, 65535, 65535, 65535, 65535, 65535, 65535, 35234, 
37444, 42634, 47854, 65535, 40615, 36546, 36546, 36483, 34209, 
35362, 40583, 42633, 47854, 65535, 36546, 36546, 33408, 33184, 
34273, 37412, 39493, 38469, 37476, 36483, 34433, 33344, 33152, 
0    , 34177, 35202, 35202, 34209, 34209, 33184, 33152, 0    
};

class BMP_ActionBtnPrev : public _constBitmap {
	public:
		BMP_ActionBtnPrev() : _constBitmap(const_cast<_pixelArray>(BMP_ActionBtnPrev_bmp), 9, 9) { }
};

static const _pixel BMP_ActionBtnShutdown_bmp[81] = {
0    , 45597, 44541, 42461, 42429, 41404, 40348, 37211, 0    , 
46621, 52958, 50813, 48733, 47677, 46621, 44541, 41372, 37142, 
44541, 50813, 48733, 65535, 65535, 65535, 41404, 41372, 38198, 
43485, 47677, 65535, 44541, 58174, 40348, 65535, 40348, 37174, 
41372, 46621, 65535, 44541, 65535, 40381, 62399, 39324, 36118, 
39292, 45565, 65535, 44541, 58174, 40413, 65535, 37244, 35095, 
37212, 43485, 44541, 65535, 65535, 65535, 37277, 34140, 34007, 
35099, 39291, 41372, 40348, 39324, 38268, 36188, 33051, 32981, 
0    , 35062, 36118, 36118, 35062, 35063, 34006, 32982, 0    
};

class BMP_ActionBtnShutdown : public _constBitmap {
	public:
		BMP_ActionBtnShutdown() : _constBitmap(const_cast<_pixelArray>(BMP_ActionBtnShutdown_bmp), 9, 9) { }
};

static const _pixel BMP_ActionBtnLogOff_bmp[81] = {
0    , 40571, 38490, 37434, 37466, 37434, 37434, 36410, 0    , 
41627, 50973, 46812, 45756, 65535, 65535, 44732, 41595, 34162, 
39547, 47836, 44732, 65535, 42651, 42684, 65535, 40570, 35219, 
39547, 45756, 43675, 41627, 65535, 40603, 65535, 40570, 35219, 
37434, 43676, 41627, 65535, 65535, 65535, 39580, 38523, 35253, 
36410, 41627, 65535, 65535, 38556, 39612, 38588, 37531, 34196, 
35354, 39547, 65535, 39579, 38556, 38588, 38588, 36507, 34228, 
33273, 38490, 39547, 38522, 38522, 38555, 37531, 36507, 33172, 
0    , 33105, 34163, 36309, 35252, 34195, 34196, 33171, 0    
};

class BMP_ActionBtnLogOff : public _constBitmap {
	public:
		BMP_ActionBtnLogOff() : _constBitmap(const_cast<_pixelArray>(BMP_ActionBtnLogOff_bmp), 9, 9) { }
};

static const _pixel BMP_ActionBtnSwitchUser_bmp[81] = {
0    , 41736, 41736, 40679, 39654, 39654, 37540, 37540, 0    , 
43786, 52050, 46893, 44811, 43787, 43786, 42761, 41736, 34241, 
39654, 47918, 44811, 65535, 43786, 42761, 41736, 39654, 35330, 
39654, 44843, 65535, 65535, 65535, 65535, 65535, 38629, 35330, 
37540, 43786, 43786, 42761, 40711, 39654, 38629, 38629, 35330, 
37540, 42761, 65535, 65535, 65535, 65535, 65535, 38629, 34369, 
34497, 40679, 40679, 39654, 38629, 65535, 38629, 37636, 33280, 
34497, 38597, 39622, 39654, 38629, 38629, 37636, 36579, 33280, 
0    , 34241, 37380, 36323, 33280, 34337, 35362, 33312, 0    
};

class BMP_ActionBtnSwitchUser : public _constBitmap {
	public:
		BMP_ActionBtnSwitchUser() : _constBitmap(const_cast<_pixelArray>(BMP_ActionBtnSwitchUser_bmp), 9, 9) { }
};

#endif