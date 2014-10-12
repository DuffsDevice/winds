#ifndef _FONT_SYSTEMFONT7_
#define _FONT_SYSTEMFONT7_

#include <_type/type.h>
#include <_type/type.font.memory.h>

/**
 * FONT_System7 font.
 */
class FONT_System7 : public _memoryFont {
	
	public:

		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or a width for monospace
		 */
		FONT_System7( _u8 fixedWidth = 0 );
};

#endif
