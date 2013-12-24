#include "_scenario/scenario.setup.h"
#include "_scenario/scenario.setup.page1.h"
#include "_scenario/scenario.setup.page2.h"
#include "_scenario/scenario.setup.page3.h"
#include "_scenario/scenario.setup.page4.h"
#include "_scenario/scenario.setup.page5.h"
#include "_type/type.system.h"
#include "_gadget/gadget.screen.startup.h"
#include "_gadget/gadget.keyboard.h"

_u8 state = 1;

_scSetup::_scSetup()
{	
	// Background
	_system::_gadgetHost_ = new _startupScreen( _system::_bgIdBack_ );
	_system::_gadgetHost_->setInternalEventHandler( onKeyClick , make_callback( this , &_scSetup::keyHandler ) );
	
	// Keyboard
	_system::_keyboard_ = new _keyboard( _system::_bgIdFront_ , _system::_gadgetHost_ , _system::_topScreen_ , 113 );
	
	this->viewSwitcher.setViewParent( _system::_gadgetHost_ );
	
	this->viewSwitcher.addView( "1" , new _scSetupPage1() );
	this->viewSwitcher.addView( "2" , new _scSetupPage2() );
	this->viewSwitcher.addView( "3" , new _scSetupPage3() );
	this->viewSwitcher.addView( "4" , new _scSetupPage4() );
	this->viewSwitcher.addView( "5" , new _scSetupPage5() );
	
	// Generate prev- and next buttons as well as their descriptions
	this->prevButton = new _actionButton( 4 , 176 , _actionButtonType::prev );
	this->prevButton->setInternalEventHandler( onMouseClick , make_callback( this , &_scSetup::prevButtonClickHandler ) );
	
	this->prevLabel = new _label( 17 , 176 , 50 , 9 , _system::getLocalizedString("lbl_prev") );
	this->prevLabel->setInternalEventHandler( onMouseClick , make_callback( this , &_scSetup::prevButtonClickHandler ) );
	this->prevLabel->setColor( RGB( 30 , 30 , 30 ) );
	
	this->nextButton = new _actionButton( 240 , 176 , _actionButtonType::next );
	this->nextButton->setInternalEventHandler( onMouseClick , make_callback( this , &_scSetup::nextButtonClickHandler ) );
	
	this->nextLabel = new _label( 188 , 176 , 50 , 9 , _system::getLocalizedString("lbl_next") );
	this->nextLabel->setInternalEventHandler( onMouseClick , make_callback( this , &_scSetup::nextButtonClickHandler ) );
	this->nextLabel->setColor( RGB( 30 , 30 , 30 ) );
	this->nextLabel->setAlign( _align::right );
	
	this->viewSwitcher.set( "1" );
	
	// Hide or show the buttons
	this->placeButtonsAndLabels();
}

_scSetup::~_scSetup()
{
	delete this->prevButton;
	delete this->nextButton;
	delete this->prevLabel;
	delete this->nextLabel;
}

void _scSetup::placeButtonsAndLabels()
{
	string state = this->viewSwitcher.getActiveView();
	
	if( state == "1" || state == "5" ){
		this->prevButton->setParent( nullptr );
		this->prevLabel->setParent( nullptr );
	}
	else{
		this->prevButton->setParent( _system::_gadgetHost_ );
		this->prevLabel->setParent( _system::_gadgetHost_ );
	}
	
	if( state == "5" ){
		this->nextButton->setParent( nullptr );
		this->nextLabel->setParent( nullptr );
	}
	else{
		this->nextButton->setParent( _system::_gadgetHost_ );
		this->nextLabel->setParent( _system::_gadgetHost_ );
	}
	
	if( state == "4" )
		this->nextLabel->setStrValue( _system::getLocalizedString("lbl_finish") );
	else
		this->nextLabel->setStrValue( _system::getLocalizedString("lbl_next") );
}

_callbackReturn _scSetup::keyHandler( _event event )
{
	if( event.getKeyCode() == DSWindows::KEY_R )
		this->nextButtonClickHandler( (_event&&)event );
	else if( event.getKeyCode() == DSWindows::KEY_L )
		this->prevButtonClickHandler( (_event&&)event );
	
	return handled;
}

_callbackReturn _scSetup::prevButtonClickHandler( _event )
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

_callbackReturn _scSetup::nextButtonClickHandler( _event )
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