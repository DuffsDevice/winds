#ifndef _BMP_TASKINFO_H_
#define _BMP_TASKINFO_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

//
// LEFT PART
//
static const _pixel BMP_TaskInfoLeftPart_bmp[90] = {
57859, 63106, 62018, 62018, 62018, 62018, 62018, 62018, 62018, 56932, 65347, 
64163, 64163, 64163, 64163, 63139, 63107, 63107, 55843, 65219, 63042, 63042, 
62018, 61986, 61986, 61986, 61986, 55843, 65251, 63107, 63074, 63042, 63042, 
62018, 61986, 61986, 55843, 65251, 64131, 63074, 63042, 63042, 61986, 61986, 
61986, 55843, 65219, 63074, 63074, 63042, 63042, 61986, 61986, 61986, 55843, 
65219, 64130, 63074, 63042, 63042, 63042, 63042, 63042, 55843, 65251, 64129, 
64130, 63074, 63074, 63073, 63073, 63073, 56931, 65379, 64226, 64194, 64162, 
64162, 64162, 64162, 64130, 57858, 63106, 60930, 60930, 59874, 59874, 59874, 
59874, 59874
};

class BMP_TaskInfoLeftPart : public _bitmap {
	public:
		BMP_TaskInfoLeftPart() : _bitmap(const_cast<_pixelArray>(BMP_TaskInfoLeftPart_bmp), 9, 10) { }
};

//
// MIDDLE PART
//

static const _pixel BMP_TaskInfoMiddlePart_bmp[10] = {
62018, 63107, 61986, 61986, 61986, 61986, 63010, 63073, 63106, 59842
};

class BMP_TaskInfoMiddlePart : public _bitmap {
	public:
		BMP_TaskInfoMiddlePart() : _bitmap(const_cast<_pixelArray>(BMP_TaskInfoMiddlePart_bmp), 1, 10) { }
};

//
// RIGHT PART
//

static const _pixel BMP_TaskInfoRightPart_bmp[60] = {
62018, 62018, 62018, 62018, 62018, 62018, 63107, 63107, 63075, 63075, 63075, 
63107, 61986, 61954, 60930, 60930, 60930, 60930, 63042, 61986, 61954, 61954, 
60930, 60930, 63010, 61986, 61986, 61986, 60930, 60930, 61986, 61986, 61986, 
61986, 61954, 60930, 63042, 61985, 61985, 61985, 61985, 61954, 63073, 63041, 
63041, 63009, 63009, 63009, 63074, 63074, 63074, 63042, 63042, 63042, 59842, 
59842, 59842, 59842, 59842, 59842
};

class BMP_TaskInfoRightPart : public _bitmap {
	public:
		BMP_TaskInfoRightPart() : _bitmap(const_cast<_pixelArray>(BMP_TaskInfoRightPart_bmp), 6, 10) { }
};

#endif