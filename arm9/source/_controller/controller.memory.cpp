#include "_controller/controller.memory.h"
#include "_controller/controller.debug.h"

//! NDS-Headers
#include <nds/dma.h>
#include <nds/arm9/cache.h>
#include <nds/interrupts.h>
#include <malloc.h>

void memset16( void* dst , _u16 val , _u32 hwn )
{
	hwn <<= 1;
	DC_FlushRange( dst , hwn );
	dmaFillHalfWords( val , dst , hwn );
}

void memcpy16( void* dst , const void* src , _u32 hwn )
{
	hwn <<= 1;
	DC_FlushRange( src , hwn );
	DC_FlushRange( dst , hwn );
	dmaCopyHalfWords( 0 , src , dst , hwn );
}

bool _memoryController::init()
{
	// Set Memory-Allocation-Error-Handler
	std::set_new_handler( &_memoryController::newHandler );
	
	// Safe the number of bytes free at the startup
	availableMemory = getFreeMemory();
	
	return true;
}

_u32 _memoryController::getFreeMemory()
{
	const _u32 memorySegment = 1024;
	
	for( _u32 s = memorySegment ; s < 4096 * 1024 ; s += memorySegment )
	{
		void *ptr = malloc(s);
		
		if( !ptr )
			return s - memorySegment;
		
		free( ptr );
	}
	
	return 0;
}

void _memoryController::newHandler()
{
	_debugController::debug("Out of memory!");
	
	while(true)
		swiWaitForVBlank();
}

_u32 _memoryController::availableMemory;