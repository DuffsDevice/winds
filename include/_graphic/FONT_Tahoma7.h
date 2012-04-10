#ifndef _FONT_TAHOMA7_
#define _FONT_TAHOMA7_

#include "_type/type.h"
#include "_type/type.font.h"

/**
 * FONT_Tahoma7 font.
 */
class FONT_Tahoma7 : public _font {
	
	public:

		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or a width for monospace
		 */
		FONT_Tahoma7( _u8 fixedWidth = 0 );
};

#endif
