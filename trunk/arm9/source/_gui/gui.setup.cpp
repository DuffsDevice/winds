#include <_gui/gui.setup.h>
#include <_gui/gui.setup.page.1.h>
#include <_gui/gui.setup.page.2.h>
#include <_gui/gui.setup.page.3.h>
#include <_gui/gui.setup.page.4.h>
#include <_gui/gui.setup.page.5.h>
#include <_screen/screen.startup.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.localization.h>

_u8 state = 1;

_guiSetup::_guiSetup()
{	
	// Background
	_guiController::enableKeyboard();
	_guiController::setHost<_startupScreen>();
	_guiController::getHost()->setInternalEventHandler( onKeyClick , make_callback( this , &_guiSetup::keyHandler ) );
	
	this->viewSwitcher.setViewParent( _guiController::getHost() );
	
	this->viewSwitcher.addView( "1" , new _guiSetupPage1() );
	this->viewSwitcher.addView( "2" , new _guiSetupPage2() );
	this->viewSwitcher.addView( "3" , new _guiSetupPage3() );
	this->viewSwitcher.addView( "4" , new _guiSetupPage4() );
	this->viewSwitcher.addView( "5" , new _guiSetupPage5() );
	
	// Generate prev- and next buttons as well as their descriptions
	this->prevButton = new _actionButton( 4 , 176 , _actionButtonType::prev );
	this->prevButton->setInternalEventHandler( onMouseClick , make_callback( this , &_guiSetup::prevButtonClickHandler ) );
	
	this->prevLabel = new _label( 17 , 176 , 50 , 9 , _localizationController::getBuiltInString("lbl_prev") );
	this->prevLabel->setInternalEventHandler( onMouseClick , make_callback( this , &_guiSetup::prevButtonClickHandler ) );
	this->prevLabel->setColor( _color::fromRGB( 30 , 30 , 30 ) );
	
	this->nextButton = new _actionButton( 240 , 176 , _actionButtonType::next );
	this->nextButton->setInternalEventHandler( onMouseClick , make_callback( this , &_guiSetup::nextButtonClickHandler ) );
	
	this->nextLabel = new _label( 188 , 176 , 50 , 9 , _localizationController::getBuiltInString("lbl_next") );
	this->nextLabel->setInternalEventHandler( onMouseClick , make_callback( this , &_guiSetup::nextButtonClickHandler ) );
	this->nextLabel->setColor( _color::fromRGB( 30 , 30 , 30 ) );
	this->nextLabel->setAlign( _align::right );
	
	this->viewSwitcher.set( "1" );
	
	// Hide or show the buttons
	this->placeButtonsAndLabels();
}

_guiSetup::~_guiSetup()
{
	delete this->prevButton;
	delete this->nextButton;
	delete this->prevLabel;
	delete this->nextLabel;
}

void _guiSetup::placeButtonsAndLabels()
{
	string state = this->viewSwitcher.getActiveView();
	
	if( state == "1" || state == "5" ){
		this->prevButton->setParent( nullptr );
		this->prevLabel->setParent( nullptr );
	}
	else{
		this->prevButton->setParent( _guiController::getHost() );
		this->prevLabel->setParent( _guiController::getHost() );
	}
	
	if( state == "5" ){
		this->nextButton->setParent( nullptr );
		this->nextLabel->setParent( nullptr );
	}
	else{
		this->nextButton->setParent( _guiController::getHost() );
		this->nextLabel->setParent( _guiController::getHost() );
	}
	
	if( state == "4" )
		this->nextLabel->setStrValue( _localizationController::getBuiltInString("lbl_finish") );
	else
		this->nextLabel->setStrValue( _localizationController::getBuiltInString("lbl_next") );
}

_callbackReturn _guiSetup::keyHandler( _event event )
{
	if( event.getKeyCode() == _key::r )
		this->nextButtonClickHandler( (_event&&)event );
	else if( event.getKeyCode() == _key::l )
		this->prevButtonClickHandler( (_event&&)event );
	
	return handled;
}

_callbackReturn _guiSetup::prevButtonClickHandler( _event )
{
	// Set previous page
	this->viewSwitcher.set(
		int2string(
			string2int( this->viewSwitcher.getActiveView() ) - 1
		)
	);
	
	// Hide or show the buttons
	this->placeButtonsAndLabels();
	
	return handled;
}

_callbackReturn _guiSetup::nextButtonClickHandler( _event )
{
	// Set next page
	this->viewSwitcher.set(
		int2string(
			string2int( this->viewSwitcher.getActiveView() ) + 1
		)
	);
	
	// Hide or show the buttons
	this->placeButtonsAndLabels();
	
	return handled;
}