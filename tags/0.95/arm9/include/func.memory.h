#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "_type/type.h"

//! NDS-Headers
#include <nds/dma.h>
#include <nds/arm9/cache.h>

unsigned int getFreeMemory();

inline void memSet( void* dst , const unsigned short int val , unsigned int hwn )
{
	DC_FlushRange( dst , hwn << 1 );
	dmaFillHalfWords( val , dst , hwn << 1 );
}

inline void memCpy( void* dst , const void* src , unsigned int hwn )
{
	DC_FlushRange( src , hwn << 1 );
	DC_FlushRange( dst , hwn << 1 );
	dmaCopyHalfWords( 0 , src , dst , hwn << 1 );
}

// Additional!
namespace DSWindows
{
	// Gets if the program runs in an emulator
	bool isEmulator();
}

#endif