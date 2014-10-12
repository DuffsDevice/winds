#include <_controller/controller.network.h>

extern "C"{
	#include <dswifi9.h>
}

bool _networkController::init(){
	return Wifi_InitDefault( INIT_ONLY );
}