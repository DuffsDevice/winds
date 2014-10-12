#ifndef _FONT_HANDWRITING9_
#define _FONT_HANDWRITING9_

#include <_type/type.h>
#include <_type/type.font.memory.h>

/**
 * FONT_System7 font.
 */
class FONT_Handwriting9 : public _memoryFont {
	
	public:

		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or a width for monospace
		 */
		FONT_Handwriting9( _u8 fixedWidth = 0 );
};

#endif
