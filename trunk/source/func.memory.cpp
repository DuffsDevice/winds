#include "func.memory.h"
#include <malloc.h>
#include <stdio.h>


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
	DC_FlushRange( dst , hwn << 1 );
	dmaFillHalfWords( val , dst , hwn << 1 );
}

void memCpy( void* dst , const void* src , unsigned int hwn )
{
	DC_FlushRange( src , hwn << 1 );
	DC_FlushRange( dst , hwn << 1 );
	dmaCopyHalfWords( 0 , src , dst , hwn << 1 );
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

string int2string( _int val )
{
	char result[32]; // string which will contain the number

	sprintf( result, "%d", val ); // %d makes the result be a decimal integer 
	
	return result;
}