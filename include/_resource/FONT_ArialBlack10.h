#ifndef _FONT_ARIALBLACK10_
#define _FONT_ARIALBLACK10_

#include "_type/type.memoryfont.h"
#include "_type/type.h"

/**
 * COURIER_NEW_FONT font.
 */
class FONT_ArialBlack10 : public _memoryfont
{
	public:

		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or a width for monospace
		 */
		FONT_ArialBlack10( _u8 fixedWidth = 0 );
};

#endif