#ifndef _BMP_REGISTRYICON_H_
#define _BMP_REGISTRYICON_H_

#include <_type/type.h>
#include <_type/type.bitmap.h>

static const _pixel BMP_RegistryIcon_bmp[100] = {
    0,     0,     0,     0,     0,     0,     0,     0, 57076,     0, 
    0,     0,     0,     0, 54961,     0,     0, 56017, 64466, 56015, 
    0,     0,     0, 61363, 65522, 51785,     0, 56013, 62345, 53903, 
56017, 54961, 57074,     0, 54950,     0,     0,     0, 53906,     0, 
56015, 65524, 54962,     0,     0,     0, 48744, 54957, 60247,     0, 
52844, 54955, 52846, 54961, 60248,     0, 53962, 65526, 48619,     0, 
57072, 65525, 55980, 65525, 54955,     0, 45510, 51751, 50733,     0, 
52844, 55980, 48745, 55980, 48744, 54957, 60247,     0,     0,     0, 
57073, 65526, 53962, 65526, 53962, 65526, 48619,     0,     0,     0, 
51848, 51816, 45510, 51784, 45510, 51751, 50733,     0,     0,     0
};

class BMP_RegistryIcon : public _constBitmap {
	public:
		BMP_RegistryIcon() : _constBitmap(const_cast<_pixelArray>(BMP_RegistryIcon_bmp), 10, 10) { }
};

#endif