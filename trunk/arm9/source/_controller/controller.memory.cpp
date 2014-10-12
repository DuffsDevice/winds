#include <_controller/controller.memory.h>
#include <_controller/controller.debug.h>

//! NDS-Headers
#include <nds/dma.h>
#include <nds/arm9/cache.h>
#include <nds/interrupts.h>
#include <malloc.h>

void memset32( void* dst , _u32 val , _u32 wordCount ){
	wordCount <<= 2;
	DC_FlushRange( dst , wordCount );
	dmaFillWords( val , dst , wordCount );
}

void memcpy32( void* dst , const void* src , _u32 wordCount ){
	wordCount <<= 2;
	DC_FlushRange( src , wordCount );
	DC_FlushRange( dst , wordCount );
	dmaCopyHalfWords( 0 , src , dst , wordCount );
}

void memset16( void* dst , _u16 val , _u32 halfWordCount ){
	halfWordCount <<= 1;
	DC_FlushRange( dst , halfWordCount );
	dmaFillHalfWords( val , dst , halfWordCount );
}

void memcpy16( void* dst , const void* src , _u32 halfWordCount ){
	halfWordCount <<= 1;
	DC_FlushRange( src , halfWordCount );
	DC_FlushRange( dst , halfWordCount );
	dmaCopyHalfWords( 0 , src , dst , halfWordCount );
}

void memset8( void* dst , _u8 val , _u32 byteCount ){
	DC_FlushRange( dst , byteCount );
	if( byteCount & 1 ){
		((_u8*)dst)[0] = val;
		((_u8*&)dst)++;
	}
	dmaFillHalfWords( val , dst , byteCount );
}

void memcpy8( void* dst , const void* src , _u32 byteCount ){
	if( byteCount & 1 ){
		((_u8*)dst)[0] = ((_u8*)src)[0];
		((_u8*&)dst)++;
		byteCount--;
	}
	DC_FlushRange( src , byteCount );
	DC_FlushRange( dst , byteCount );
	dmaCopyHalfWords( 0 , src , dst , byteCount );
}

bool _memoryController::init()
{
	// Set Memory-Allocation-Error-Handler
	std::set_new_handler( &_memoryController::newHandler );
	
	// Safe the number of bytes free at the startup
	availableMemory = getFreeMemory();
	
	// Display Memory Results at end of program
	std::atexit( &_memoryAnalyzer::printResults );
	
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