#ifndef _FONT_COURIERNEW10_
#define _FONT_COURIERNEW10_

#include "_type/type.font.memory.h"
#include "_type/type.h"

/**
 * COURIER_NEW_FONT font.
 */
class FONT_CourierNew10 : public _memoryFont
{
	public:

		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or a width for monospace
		 */
		FONT_CourierNew10( _u8 fixedWidth = 0 );
};

#endif