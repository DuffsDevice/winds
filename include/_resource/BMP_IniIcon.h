#ifndef _BMP_INIICON_H_
#define _BMP_INIICON_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_IniIcon_bmp[110] = {
53908, 53908, 53908, 53908, 53908, 53908, 53908, 53908, 53908, 53908, 53908, 
53908, 63421, 63421, 63421, 63421, 63421, 63421, 63421, 63421, 63421, 53908, 
53908, 63421, 63421, 36996, 38053, 63421, 63421, 63421, 63421, 63421, 53908, 
53908, 34882, 63421, 36996, 38053, 63421, 40167, 40167, 63421, 63421, 53908, 
53908, 34882, 35939, 36996, 38053, 39110, 40167, 40167, 63421, 63421, 53908, 
53908, 34882, 35939, 61307, 61307, 39110, 40167, 63421, 63421, 63421, 53908, 
53908, 34882, 61307, 36996, 38053, 61307, 40167, 40167, 40167, 63421, 53908, 
53908, 34882, 61307, 36996, 38053, 61307, 40167, 40167, 40167, 63421, 53908, 
53908, 34882, 35939, 61307, 61307, 39110, 40167, 63421, 63421, 63421, 53908, 
53908, 53908, 53908, 53908, 53908, 53908, 53908, 53908, 53908, 53908, 53908
};

class BMP_IniIcon : public _constbitmap {
	public:
		BMP_IniIcon() : _constbitmap(const_cast<_pixelArray>(BMP_IniIcon_bmp), 11, 10) { }
};

#endif
