#ifndef _FONT_SYSTEM_SYMBOLS_8_
#define _FONT_SYSTEM_SYMBOLS_8_

#include <_type/type.h>
#include <_type/type.font.memory.h>

/**
 * FONT_SystemSymbols8 font.
 */
class FONT_SystemSymbols8 : public _memoryFont {
	
	public:

		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or a width for monospace
		 */
		FONT_SystemSymbols8( _u8 fixedWidth = 0 );
};

#endif
