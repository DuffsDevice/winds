#ifndef _WIN_C_INPUT_
#define _WIN_C_INPUT_

#include "_type/type.controller.h"
#include "_type/type.h"

class _inputController : public _controller
{
	private:
		
		static _u32 heldCycles[16];
		
	public:
		
		//! Inits the interrupt system
		static bool init();
		
		//! Performs one processing step
		static void frame();
};

#endif