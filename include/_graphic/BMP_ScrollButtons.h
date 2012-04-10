#ifndef _BMP_SCROLLBUTTONS_H_
#define _BMP_SCROLLBUTTONS_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_ScrollButtonTop_bmp[64] = {
65534, 65468, 65435, 65435, 65435, 65435, 65468, 63454, 
65468, 65435, 65402, 65401, 65368, 65368, 64311, 63388, 
65435, 65401, 65369, 61173, 65335, 65335, 64311, 62330, 
65435, 65401, 62197, 49545, 53773, 65334, 64311, 62329, 
64411, 62197, 49545, 60115, 52716, 53773, 64310, 62329, 
65402, 65368, 60115, 65368, 65366, 60082, 64310, 62329, 
64444, 64377, 64377, 64377, 64377, 64377, 64410, 62330, 
63354, 61205, 61172, 61172, 61172, 61172, 62229, 63421, 
};

class BMP_ScrollButtonTop : public _bitmap {
	public:
		BMP_ScrollButtonTop() : _bitmap(const_cast<_pixelArray>(BMP_ScrollButtonTop_bmp), 8, 8) { }
};

//
// Bottom
//

static const _pixel BMP_ScrollButtonBottom_bmp[64] = {
65469, 65434, 65401, 65368, 64344, 64311, 64311, 63420, 
65468, 65402, 65401, 65368, 65335, 65335, 64311, 63355, 
65435, 65401, 60116, 65368, 65335, 60082, 64311, 62330, 
64411, 62197, 49545, 60115, 52684, 53773, 64310, 62329, 
65434, 65368, 62197, 49545, 53773, 65334, 64310, 62329, 
64378, 65335, 65367, 61172, 65366, 65334, 64277, 62329, 
65435, 64310, 64311, 64310, 64310, 64278, 64311, 62330, 
65468, 63320, 62263, 62263, 62263, 62263, 63320, 62363
};

class BMP_ScrollButtonBottom : public _bitmap {
	public:
		BMP_ScrollButtonBottom() : _bitmap(const_cast<_pixelArray>(BMP_ScrollButtonBottom_bmp), 8, 8) { }
};


//
// Left
//

static const _pixel BMP_ScrollButtonLeft_bmp[64] = {
65468, 65435, 64378, 65434, 64411, 65435, 65468, 65469, 
63320, 64310, 65335, 65368, 62197, 65401, 65402, 65434, 
62263, 64311, 65367, 62197, 49545, 60116, 65401, 65401, 
62263, 64310, 61172, 49545, 60115, 65368, 65368, 65368, 
62263, 64310, 65366, 53773, 52684, 65335, 65335, 64344, 
62263, 64278, 65334, 65334, 53773, 60082, 65335, 64311, 
63320, 64311, 64277, 64310, 64310, 64311, 64311, 64311, 
62363, 62330, 62329, 62329, 62329, 62330, 63355, 63420,
};

class BMP_ScrollButtonLeft : public _bitmap {
	public:
		BMP_ScrollButtonLeft() : _bitmap(const_cast<_pixelArray>(BMP_ScrollButtonLeft_bmp), 8, 8) { }
};

//
// Right
//

static const _pixel BMP_ScrollButtonRight_bmp[64] = {
64444, 63354, 65402, 64411, 65435, 65435, 65468, 65534, 
64311, 64377, 65368, 62197, 65401, 65401, 65435, 65468, 
64311, 64377, 60115, 49545, 62197, 65369, 65402, 65435, 
64311, 64377, 65368, 60115, 49545, 61173, 65401, 65435, 
64311, 64377, 65366, 52716, 53773, 65335, 65368, 65435, 
64311, 64377, 60082, 53773, 65334, 65335, 65368, 65435, 
62229, 64410, 64310, 64310, 64311, 64311, 64311, 65468, 
63421, 62330, 62329, 62329, 62329, 62330, 63388, 63454
};

class BMP_ScrollButtonRight : public _bitmap {
	public:
		BMP_ScrollButtonRight() : _bitmap(const_cast<_pixelArray>(BMP_ScrollButtonRight_bmp), 8, 8) { }
};


//
// Scroll Background
//

static const _pixel BMP_ScrollBg_bmp[8] = {
	62364, 63454, 63454, 64478, 64511, 64511, 65535, 63454
};

class BMP_ScrollBg : public _bitmap {
	public:
		BMP_ScrollBg() : _bitmap(const_cast<_pixelArray>(BMP_ScrollBg_bmp), 1, 8) { }
};

class BMP_ScrollBg_ : public _bitmap {
	public:
		BMP_ScrollBg_() : _bitmap(const_cast<_pixelArray>(BMP_ScrollBg_bmp), 8, 1) { }
};


#endif