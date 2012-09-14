#include "func.memory.h"
#include "malloc.h"

#include <nds/dma.h>
#include <nds/arm9/cache.h>

unsigned int getFreeMemory()
{
	const unsigned int FreeMemSeg=2*1024;
  
	unsigned int i;
	for( i = FreeMemSeg ; i < 4096 * 1024 ; i += FreeMemSeg )
	{
		void *ptr=malloc( i );
		if( ptr == 0 )
			return i-FreeMemSeg;
		free( ptr );
		ptr = 0;
	}
}

void memSet( void* dst , const unsigned short int val , unsigned int hwn )
{
	hwn <<= 1;
	DC_FlushRange( dst , hwn );
	dmaFillHalfWords( val , dst , hwn );
}

void memCpy( void* dst , const void* src , unsigned int hwn )
{
	hwn <<= 1;
	DC_FlushRange( src , hwn );
	dmaCopyHalfWords( 0 , src , dst , hwn );
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