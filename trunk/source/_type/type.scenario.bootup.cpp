#include "_type/type.scenario.bootup.h"
#include "_type/type.system.controller.h"
#include "_gadget/gadget.bootupScreen.h"

// Callback to change systemState
void callback()
{
	_systemController::changeState( _systemController::_systemState::login );
}


_scBootup::_scBootup()
{
	// Create BootupScreen
	_system::_gadgetHost_ = new _bootupScreen( _system::_bgIdBack_ );
	
	_system::executeTimer( new _staticCallback( callback ) , 2000 );
}

_scBootup::~_scBootup()
{
	
}

