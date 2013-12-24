#ifndef _BMP_SHORTCUTOVERLAY_H_
#define _BMP_SHORTCUTOVERLAY_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_ShortcutOverlay_bmp[25] = {
32768, 32768, 32768, 32768, 32768, 
32768, 65535, 65535, 65535, 65535, 
32768, 65535, 32768, 32768, 65535, 
32768, 65535, 32768, 58136, 65535, 
32768, 65535, 65535, 32768, 65535
};

class BMP_ShortcutOverlay : public _constBitmap {
	public:
		BMP_ShortcutOverlay() : _constBitmap(const_cast<_pixelArray>(BMP_ShortcutOverlay_bmp), 5, 5) { }
};

#endif