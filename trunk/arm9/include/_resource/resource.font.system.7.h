#ifndef _FONT_SYSTEM_7_
#define _FONT_SYSTEM_7_

#include <_type/type.h>
#include <_type/type.font.virtual.h>

/**
 * FONT_System7 font.
 */
class FONT_System7 : public _virtualFont
{
	public:
		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or a width for monospace
		 */
		FONT_System7( _u8 fixedWidth = 0 );
};

#endif