#ifndef _BMP_LUAICON_H_
#define _BMP_LUAICON_H_

#include "_type/type.h"
#include "_type/type.bitmap.h"

static const _pixel BMP_LuaIcon_bmp[100] = {
65535, 65535, 63104, 49152, 49152, 49152, 49152, 63347, 63104, 49152, 65535, 
63104, 49152, 49152, 49152, 49152, 65535, 49152, 63347, 63104, 63104, 49152, 
49152, 49152, 49152, 49152, 65535, 65535, 49152, 63347, 56090, 49152, 49152, 
49152, 49152, 49152, 49152, 49152, 49152, 49152, 56090, 49152, 49152, 56090, 
49152, 56090, 49152, 49152, 56090, 56090, 56090, 49152, 49152, 56090, 49152, 
56090, 49152, 56090, 49152, 56090, 56090, 56090, 49152, 56090, 56090, 56090, 
49152, 49152, 56090, 56090, 63104, 49152, 49152, 49152, 49152, 49152, 49152, 
49152, 49152, 63104, 65535, 63104, 49152, 49152, 49152, 49152, 49152, 49152, 
63104, 65535, 65535, 65535, 63104, 49152, 49152, 49152, 49152, 63104, 65535, 
65535
};

class BMP_LuaIcon : public _constBitmap {
	public:
		BMP_LuaIcon() : _constBitmap(const_cast<_pixelArray>(BMP_LuaIcon_bmp), 10 , 10 ) { }
};

#endif