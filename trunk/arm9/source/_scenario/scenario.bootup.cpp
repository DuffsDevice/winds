#include "_scenario/scenario.bootup.h"
#include "_type/type.system.controller.h"
#include "_gadget/gadget.bootupScreen.h"

// Callback to change systemState
void callback()
{
	_systemController::changeState( _systemState::login );
}


_scBootup::_scBootup() :
	timer( make_callback( callback ) , 2000 , false )
{
	// Create BootupScreen
	_system::_gadgetHost_ = new _bootupScreen( _system::_bgIdBack_ );
}