#ifndef _FONT_SYSTEM_ASCII_7_
#define _FONT_SYSTEM_ASCII_7_

#include <_type/type.h>
#include <_type/type.font.memory.h>

/**
 * FONT_SystemASCII7 font.
 */
class FONT_SystemASCII7 : public _memoryFont {
	
	public:

		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or a width for monospace
		 */
		FONT_SystemASCII7( _u8 fixedWidth = 0 );
};

#endif
