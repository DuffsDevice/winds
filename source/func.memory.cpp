#include "func.memory.h"

void memCopy( _pixelArray dest , _pixelArray src , _u32 size )
{
	// Duff, bitch!
	/*switch( size % 8 ){
		case 0:	do{		*dest++ = *src++;
		case 7:			*dest++ = *src++;
		case 6:			*dest++ = *src++;
		case 5:			*dest++ = *src++;
		case 4:			*dest++ = *src++;
		case 3:			*dest++ = *src++;
		case 2:			*dest++ = *src++;
		case 1:			*dest++ = *src++;
				}while( (size-=8) > 0 );
	}*/
	while( size-- > 0 ){
		*dest++ = *src++;
	};
}

void memSet( _pixelArray dest , _s32 value , _u32 size )
{
	// Hi Duff! ....bitch!
	/*switch( size % 8 ){
		case 0:	do{		*dest++ = value;
		case 7:			*dest++ = value;
		case 6:			*dest++ = value;
		case 5:			*dest++ = value;
		case 4:			*dest++ = value;
		case 3:			*dest++ = value;
		case 2:			*dest++ = value;
		case 1:			*dest++ = value;
				}while( (size-=8) > 0 );
	}*/
	while( size-- > 0 ){
		*dest++ = value;
	};
}

#include "malloc.h"

_u32 getFreeMemory()
{
	const _u32 FreeMemSeg=2*1024;
  
	for( _u32 i = FreeMemSeg ; i < 4096 * 1024 ; i += FreeMemSeg )
	{
		void *ptr=malloc( i );
		if( ptr == NULL )
			return i-FreeMemSeg;
		free( ptr );
		ptr = NULL;
	}
}

int string2int(const char *p){
    int x = 0;
    bool neg = false;
    if (*p == '-') {
        neg = true;
        ++p;
    }
    while (*p >= '0' && *p <= '9') {
        x = (x*10) + (*p - '0');
        ++p;
    }
    if (neg) {
        x = -x;
    }
    return x;
}