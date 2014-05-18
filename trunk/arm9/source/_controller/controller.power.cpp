#include "_controller/controller.power.h"

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