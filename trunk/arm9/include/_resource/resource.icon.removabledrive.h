#ifndef _BMP_REMOVABLEDRIVEICON_H_
#define _BMP_REMOVABLEDRIVEICON_H_

#include <_type/type.h>
#include <_type/type.bitmap.h>

static const _pixel BMP_RemovableDriveIcon_bmp[100] = {
    0,     0, 60250, 56022, 56022, 56022, 54965, 56022,     0,     0,
    0, 60250, 62364, 62364, 62364, 62364, 62364, 62364, 54965,     0, 
54965, 62364, 62364, 61307, 61307, 61307, 61307, 61307, 61307, 48623, 
54965, 60250, 56022, 56022, 56022, 56022, 56022, 56022, 59193, 48623, 
54965, 61307, 50737, 50737, 50737, 50737, 50737, 50737, 59193, 48623, 
54965, 62364, 58136, 58136, 58136, 58136, 58136, 58136, 62364, 46509, 
57079, 48623, 48623, 48623, 48623, 48623, 48623, 46509, 46509, 51794, 
    0,     0,     0,     0, 46509, 46509,     0,     0,     0,     0, 
46509, 46509, 46509, 46509, 36758, 36758, 46509, 46509, 46509, 46509, 
    0,     0,     0, 36758, 42692, 42692, 36758,     0,     0,     0
};

class BMP_RemovableDriveIcon : public _constBitmap {
	public:
		BMP_RemovableDriveIcon() : _constBitmap(const_cast<_pixelArray>(BMP_RemovableDriveIcon_bmp), 10 , 10 ) { }
};

#endif