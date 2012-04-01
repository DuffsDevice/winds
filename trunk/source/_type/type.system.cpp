#include "_type/type.system.h"
#include <nds.h>
#include <time.h>

#define transfer (*(__TransferRegion volatile *)(0x02FFF000))

struct __TransferRegion {
	vs16 touchX,   touchY;		// TSC X, Y
	vs16 touchXpx, touchYpx;	// TSC X, Y pixel values
	vs16 touchZ1,  touchZ2;		// TSC x-panel measurements
	vu16 buttons;				// X, Y, /PENIRQ buttons
	time_t	unixTime;
	struct __bootstub *bootcode;
};

void _system::setBacklight( _u8 level ){
	
}

void _system::restart(){
	
	struct __bootstub *bootcode = transfer.bootcode;
	
	bootcode->arm9reboot();

}

void _system::shutDown(){
	systemShutDown(); 
}

bool initFat(){
	return fatInitDefault();
}