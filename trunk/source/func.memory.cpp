#include "func.memory.h"
#include <malloc.h>


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
	
	return 0;
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