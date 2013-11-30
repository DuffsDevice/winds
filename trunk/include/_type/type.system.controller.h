#ifndef _WIN_T_SYSTEMCONTROLLER_
#define _WIN_T_SYSTEMCONTROLLER_

#include "_type/type.h"
#include "_type/type.scenario.h"
#include "_scenario/scenario.setup.h"
#include "_scenario/scenario.bootup.h"
#include "_scenario/scenario.desktop.h"
#include "_scenario/scenario.login.h"

enum class _systemState : _u8
{
	empty,
	login,
	bootup,
	setup,
	desktop
};

class _systemController
{
	private:
		
		static _systemState state;
		static bool			dirty;
		
		//! The Scenario-Object that is having controll
		//! over the system right now
		static _scenario*	scenario;
	
	public:
		
		//! The current state - getter and setter
		static void changeState( _systemState state ){ dirty = _systemController::state != state; _systemController::state = state; }
		static _systemState getState(){ return state; }
		
		//! Method that will be called after the Hardware initialization is done
		//! This Routine does nearly everything
		static void main();
		
		//! Method that changes the current state if requested
		static void checkDirty();
};

#endif