#ifndef _BMP_ARCHIVEICON_H_
#define _BMP_ARCHIVEICON_H_

#include <_type/type.h>
#include <_type/type.bitmap.h>

static const _pixel BMP_ArchiveIcon_bmp[90] = {
    0, 53841, 53841, 53841, 53841, 53841, 53841, 53841,     0,     0, 52783, 
65499, 65531, 65499, 61271, 50670, 61271,     0, 38521, 37297, 37297, 37297, 
37297,     0, 50670, 53873,     0, 38521, 57247, 65535, 57247, 38389,     0, 
53841, 61271,     0, 38521, 37297, 37297, 37297, 38389,     0, 50670, 53873, 
    0, 38521, 57247, 65535, 57247, 38389,     0, 53841, 61271,     0, 38389, 
37297, 37297, 37297, 37297,     0, 50670, 53873,     0,     0, 52783, 53841, 
53841, 53841, 61271, 50670, 61271,     0,     0,     0,     0,     0, 47499, 
53841, 53841, 53841, 47499,     0,     0,     0,     0, 47499, 47499, 47499, 
47499, 47499
};

class BMP_ArchiveIcon : public _constBitmap {
	public:
		BMP_ArchiveIcon() : _constBitmap(const_cast<_pixelArray>(BMP_ArchiveIcon_bmp), 9 , 10 ) { }
};

#endif