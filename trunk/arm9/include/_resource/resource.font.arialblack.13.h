#ifndef _FONT_ARIALBLACK13_
#define _FONT_ARIALBLACK13_

#include <_type/type.font.memory.h>
#include <_type/type.h>

/**
 * COURIER_NEW_FONT font.
 */
class FONT_ArialBlack13 : public _memoryFont
{
	public:

		/**
		 * Constructor.
		 * @param fixedWidth Set to 0 for proportional or a width for monospace
		 */
		FONT_ArialBlack13( _u8 fixedWidth = 0 );
};

#endif