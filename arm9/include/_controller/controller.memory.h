#ifndef _WIN_C_MEMORY_
#define _WIN_C_MEMORY_

#include <_type/type.h>
#include <_type/type.controller.h>

void memset32( void* dest , _u32 val , _u32 wordCount );
void memcpy32( void* dest , const void* src , _u32 wordCount );
void memset16( void* dest , _u16 val , _u32 halfWordCount );
void memcpy16( void* dest , const void* src , _u32 halfWordCount );
void memset8( void* dest , _u8 val , _u32 byteCount );
void memcpy8( void* dest , const void* src , _u32 byteCount );

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