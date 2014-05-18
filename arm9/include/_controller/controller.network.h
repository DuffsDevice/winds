#ifndef _WIN_C_NETWORK_
#define _WIN_C_NETWORK_

#include "_type/type.h"
#include "_type/type.controller.h"

class _networkController : public _controller
{
	public:
		
		//! Inits the interrupt system
		static bool init();
};

#endif