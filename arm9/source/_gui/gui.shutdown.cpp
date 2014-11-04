#include <_gui/gui.shutdown.h>
#include <_screen/screen.startup.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.debug.h>
#include <_controller/controller.localization.h>
#include <_controller/controller.registry.h>
#include <_type/type.windows.h>
#include <_resource/resource.image.windows.startuplogo.h>

_guiShutdown::_guiShutdown() :
	timer(
		make_inline_callback<void()>(
			[this]()
			{
				if( _registryController::isUserLoggedIn() )
				{
					// Indicate we will log off the user
					this->shutdownLabel->setStrValue( _localizationController::getBuiltInString("lbl_logging_off") );
					
					// Log of user
					_registryController::setUserRegistry( nullptr );
				}
				
				// Indicate we will shut down
				this->shutdownLabel->setStrValue( _localizationController::getBuiltInString("lbl_windows_shutting_down") );
				
				// Shut down
				_windows::stop();
			}
		)
		, 400 , false
	)
{
	// Create Windows
	_guiController::setHost<_startupScreen>();
	_guiController::disableKeyboard();
	
	this->shutdownLabel = new _label( 23 , 93 , 130 , 20 , "" );
	this->shutdownLabel->setColor( _color::white );
	this->shutdownLabel->setAlign( _align::right );
	
	this->windowsLogo = new _imageGadget( 106 , 43 , BMP_WindowsStartUpLogo() );
	
	_guiController::getHost()->addChild( this->shutdownLabel );
	_guiController::getHost()->addChild( this->windowsLogo );
	
	this->timer.start();
}

_guiShutdown::~_guiShutdown(){
	delete this->windowsLogo;
	delete this->shutdownLabel;
}