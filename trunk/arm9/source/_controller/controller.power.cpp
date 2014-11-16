#include <_controller/controller.power.h>

extern "C"{
	#include <nds/system.h>
}

bool _powerController::init(){
	powerOn( POWER_ALL ); // Power on everything
	return true;
}

void _powerController::shutDown(){
	systemShutDown();
}

int _powerController::getBatteryLevel(){
	return getBatteryLevel();
}

void _powerController::sleep(){
	systemSleep();
}

void _powerController::end()
{
	// Call handlers
	for( _uniquePtr<_callback<void(void)>>& func : beforeShutdownHandlers )
		(*func.get())();
	
	// Shut down the system
	shutDown();
}

_vector<_uniquePtr<_callback<void(void)>>> _powerController::beforeShutdownHandlers;