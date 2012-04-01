#include "DSWindows.h"

namespace DSWindows
{	
	void memcpyHalfWords( _u16* src , _u16* dest , _u32 size ){
		while(size--)
			*src++ = *dest++;
	}
	
}