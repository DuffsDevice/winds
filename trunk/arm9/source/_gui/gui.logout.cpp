#include <_gui/gui.logout.h>
#include <_screen/screen.startup.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.debug.h>
#include <_controller/controller.localization.h>
#include <_controller/controller.registry.h>
#include <_resource/resource.image.windows.startuplogo.h>

_guiLogout::_guiLogout() :
	timer(
		make_inline_callback<void()>(
			[this]()
			{
				// Indicate we will log off the user
				this->logoutLabel->setStrValue( _localizationController::getBuiltInString("lbl_logging_off") );
				
				// Log of user
				_registryController::setUserRegistry( nullptr );
				
				// Go back to login
				_guiController::changeState( _guiState::login );
			}
		)
		, 400 , false
	)
{
	// Create Windows
	_guiController::setHost<_startupScreen>();
	_guiController::disableKeyboard();
	
	this->logoutLabel = new _label( 28 , 98 , 130 , ignore , "" );
	this->logoutLabel->setColor( _color::white );
	this->logoutLabel->setAlign( _align::right );
	
	this->windowsLogo = new _imageGadget( 106 , 43 , BMP_WindowsStartUpLogo() );
	
	// Check if there is a user to log out
	if( !_registryController::isUserLoggedIn() ){
		_guiController::changeState( _guiState::login );
		return;
	}
	
	_guiController::getHost()->addChild( this->logoutLabel );
	_guiController::getHost()->addChild( this->windowsLogo );
	
	this->timer.start();
}

_guiLogout::~_guiLogout(){
	delete this->windowsLogo;
	delete this->logoutLabel;
}