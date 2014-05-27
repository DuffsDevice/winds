#ifndef _WIN_C_MEMORY_
#define _WIN_C_MEMORY_

#include "_type/type.h"
#include "_type/type.controller.h"

void memset16( void* dst , _u16 val , _u32 hwn );
void memcpy16( void* dst , const void* src , _u32 hwn );

class _memoryController : public _controller
{
	private:
		
		static _u32 availableMemory;
		
		//! Handler for an uncought outOfMemory-exception
		static void newHandler();
		
	public:
		
		//! Inits the interrupt system
		static bool init();
		
		//! Get avaliable memory in bytes
		static _u32 getAvailableMemory(){ return availableMemory; }
		
		//! Get free memory in bytes
		static _u32 getFreeMemory();
};

#endif