#ifndef _WIN_T_SYSTEMSCENARIO_
#define _WIN_T_SYSTEMSCENARIO_


class _scenario
{
	public:
		
		// Virtual Dtor
		virtual ~_scenario(){ };
		
};

// Includes all types of scenarios
#include "_type/type.scenario.login.h"
#include "_type/type.scenario.setup.h"
#include "_type/type.scenario.bootup.h"
#include "_type/type.scenario.desktop.h"

#endif