#ifndef _FONT_SYSTEMSYMBOLS8_
#define _FONT_SYSTEMSYMBOLS8_

#include "_type/type.h"
#include "_type/type.memoryfont.h"

/**
 * FONT_SystemSymbols8 font.
 */
class FONT_SystemSymbols8 : public _memoryfont {
	
	public:

		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or a width for monospace
		 */
		FONT_SystemSymbols8( _u8 fixedWidth = 0 );
};

#endif
