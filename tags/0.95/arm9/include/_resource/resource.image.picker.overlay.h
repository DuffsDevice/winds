#ifndef _BMP_COLORPICKER_OVERLAY_H_
#define _BMP_COLORPICKER_OVERLAY_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_ColorPickerOverlay_bmp[100] = {
    0,     0,     0,     0, 65535, 65535,     0,     0,     0,     0, 
    0,     0, 65535, 65535, 32768, 32768, 65535, 65535,     0,     0, 
65535, 65535, 32768, 32768,     0,     0, 32768, 32768, 65535, 65535, 
65535, 32768,     0,     0,     0,     0,     0,     0, 32768, 65535, 
65535, 32768,     0,     0,     0,     0,     0,     0, 32768, 65535, 
65535, 32768,     0,     0,     0,     0,     0,     0, 32768, 65535, 
65535, 32768,     0,     0,     0,     0,     0,     0, 32768, 65535, 
65535, 65535, 32768, 32768,     0,     0, 32768, 32768, 65535, 65535, 
    0,     0, 65535, 65535, 32768, 32768, 65535, 65535,     0,     0, 
    0,     0,     0,     0, 65535, 65535,     0,     0,     0,     0
};

class BMP_ColorPickerOverlay : public _constBitmap {
	public:
		BMP_ColorPickerOverlay() : _constBitmap(const_cast<_pixelArray>(BMP_ColorPickerOverlay_bmp), 10, 10) { }
};

#endif