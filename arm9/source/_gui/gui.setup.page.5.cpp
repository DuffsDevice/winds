#include <_gui/gui.setup.page.5.h>
#include <_gui/gui.setup.page.4.h>
#include <_gui/gui.setup.page.3.h>
#include <_controller/controller.localization.h>
#include <_type/type.user.h>

void _guiSetupPage5::timerCallback()
{
	_viewSwitcher& switcher = this->getSwitcher();
	
	//_guiSetupPage3* page3 = (_guiSetupPage3*) switcher.getViewByName( "3" );
	_guiSetupPage4* page4 = (_guiSetupPage4*) switcher.getViewByName( "4" );
	
	string profileName = page4->profileName;
	
	_user user = _user( profileName );
	user.setLogo( page4->profileIcon );
}

void _guiSetupPage5::create( _gadget* viewParent )
{
	timer = _timer( make_callback( this , &_guiSetupPage5::timerCallback ) , 0 , false );
	
	// Label
	this->lblInitializingProgress = new _label( 0 , 60 , ignore , ignore , _localizationController::getBuiltInString("lbl_wait_installing") );
	this->lblInitializingProgress->setColor( _color::fromRGB( 30 , 30 , 30 ) );
	this->lblInitializingProgress->setAlign( _align::center );
	this->lblInitializingProgress->setWidth( SCREEN_WIDTH );
	viewParent->addChild( this->lblInitializingProgress );
	
	// Progressbar
	this->progressBar = new _progressBar( 101 , 72 , 50 , false );
	viewParent->addChild( this->progressBar );
	
	// Start execution
	timer.start();
}

bool _guiSetupPage5::destroy( _gadget* viewParent )
{
	delete this->lblInitializingProgress;
	
	// Switching the view is allowed
	return true;
}