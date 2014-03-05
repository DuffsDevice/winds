#include "func.memory.h"
#include <malloc.h>

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

// Gets if the program runs in an emulator
bool DSWindows::isEmulator()
{
	// The idea behind the code is to overwrite
	// the "mov r0, #0" instruction with "mov r0, r0" (NOP).
	// On real hardware, the instruction would have been fetched,
	// so the overwrite has no effect for the first time executed.
	// In order to be able to call the function more than once,
	// the original instruction is being restored.
	// see http://forum.gbadev.org/viewtopic.php?t=910
	#ifdef __thumb__
		int mov_r0_r0 = 0x1c00;   // mov r0, r0
		int mov_r0_0  = 0x2000;   // mov r0, #0
		
		asm volatile (
		"mov  r0, %1     \n\t"  // r0 = mov_r0_r0
		"mov  r2, %2     \n\t"  // r2 = mov_r0_0
		"mov  r1, pc     \n\t"  // r1 = program counter
		"strh r0, [r1]   \n\t"  // Overwrites following instruction with mov_r0_r0
		"mov  r0, #0     \n\t"  // r0 = 0
		"strh r2, [r1]   \n\t"  // Restore previous instruction
		: "=r"(mov_r0_r0)       // output registers
		: "r"(mov_r0_r0), "r"(mov_r0_0)    // input registers
		: "%r1","%r2"           // clobbered registers
		);
	#else
		int mov_r0_r0 = 0xe1a00000; // mov r0, r0
		int mov_r0_0  = 0xe3a00000; // mov r0, #0
		
		asm volatile (
		"mov  r0, %1     \n\t"  // r0 = mov_r0_r0
		"mov  r2, %2     \n\t"  // r2 = mov_r0_0
		"mov  r1, pc     \n\t"  // r1 = program counter
		"str  r0, [r1]   \n\t"  // Overwrites following instruction with mov_r0_r0
		"mov  r0, #0     \n\t"  // r0 = 0
		"str  r2, [r1]   \n\t"  // Restore previous instruction
		: "=r"(mov_r0_r0)       // output registers
		: "r"(mov_r0_r0), "r"(mov_r0_0)    // input registers
		: "%r1","%r2"           // clobbered registers
		);
	#endif

	return mov_r0_r0 != 0;
}