#include <_gui/gui.bootup.h>
#include <_screen/screen.bootup.h>
#include <_controller/controller.gui.h>

// Callback to change systemState
void _guiBootup::timerCallback(){
	_guiController::changeState( _guiState::login );
}


_guiBootup::_guiBootup() :
	timer( make_callback( &_guiBootup::timerCallback ) , 2000 , false )
{
	// Create BootupScreen
	_guiController::setHost<_bootupScreen>();
	_guiController::disableKeyboard();
}