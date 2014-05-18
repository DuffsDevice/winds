#ifndef _WIN_T_CONTROLLER_
#define _WIN_T_CONTROLLER_

using _controllerInit = bool();
using _controllerFrame = void();
using _controllerEnd = void();

class _controller
{
	public:
		
		// Fallback methods
		static bool init(){ return true; };
		static void frame(){};
		static void end(){};
};

#endif