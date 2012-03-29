#ifndef _BMP_XMLICON_H_
#define _BMP_XMLICON_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_XmlIcon_bmp[80] = {
49680, 49680, 49680, 49680, 49680, 49680, 0	   , 0	  , 49680, 65535, 65535, 
65535, 65535, 49680, 49680, 0	 , 49680, 65535, 65535, 65535, 65535, 49680, 
49680, 49680, 49680, 65535, 65535, 63347, 63347, 65535, 65535, 49680, 49680, 
65535, 63104, 65535, 65535, 63104, 65535, 49680, 49680, 63104, 65535, 65535, 
65535, 65535, 63104, 49680, 49680, 65535, 63104, 65535, 65535, 63104, 65535, 
49680, 49680, 65535, 65535, 63347, 63347, 65535, 65535, 49680, 49680, 65535, 
65535, 65535, 65535, 65535, 65535, 49680, 49680, 49680, 49680, 49680, 49680, 
49680, 49680, 49680
};

class BMP_XmlIcon : public _bitmap {
	public:
		BMP_XmlIcon() : _bitmap(const_cast<_pixelArray>(BMP_XmlIcon_bmp), 8, 10) { }
};

#endif