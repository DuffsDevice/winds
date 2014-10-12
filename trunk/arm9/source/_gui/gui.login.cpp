#include <_gui/gui.login.h>
#include <_controller/controller.localization.h>
#include <_controller/controller.gui.h>
#include <_controller/controller.font.h>
#include <_controller/controller.registry.h>

//! Gadgets we need
#include <_gadget/gadget.label.h>
#include <_screen/screen.startup.h>


_callbackReturn _guiLogin::userLoginHandler( _event event )
{
	_userWrapper* that = event.getGadget<_userWrapper>();
	
	// Delete all wrappers but the supplied one
	this->userWrapper.remove_if( [=]( _userWrapper* wrapper ){ if( that != wrapper ){ delete wrapper; return true; } return false; } );
	
	// Blur the currently focused wrapper
	that->blur();
	
	// Prevent it from receiving focus again
	that->applyStyle( _style::notFocusable );
	
	this->welcome = new _label( 13 , 79 , 105 , 20 , _localizationController::getBuiltInString("lbl_welcome") );
	this->welcomeBg = new _label( 12 , 78 , 105 , 20 , _localizationController::getBuiltInString("lbl_welcome") );
	
	this->welcome->setFont( _fontController::getFont( "ArialBlack13" ) );
	this->welcomeBg->setFont( _fontController::getFont( "ArialBlack13" ) );
	this->welcome->setColor( _color::fromRGB( 2 , 5 , 15 ) );
	this->welcomeBg->setColor( _color::fromRGB( 30 , 30 , 30 ) );
	this->welcome->setAlign( _align::right );
	this->welcomeBg->setAlign( _align::right );
	
	_guiController::getHost()->addChild( welcome );
	_guiController::getHost()->addChild( welcomeBg );
	
	this->loginAnim.setFromValue( that->getY() );
	this->loginAnim.start();
	this->loginTmr.start();
	
	return handled;
}


void _guiLogin::loginAnimSetter( int val ){
	this->userWrapper.front()->setY( val );
}


void _guiLogin::loginSwitchDesktop()
{
	// Log the User in
	_registryController::setUserRegistry( this->userWrapper.front()->getPath() );
	
	// Change state to user
	_guiController::changeState( _guiState::desktop );
}


_guiLogin::_guiLogin() :
	loginAnim( 0 , 0 , 1000 )
	, loginTmr( make_callback( this , &_guiLogin::loginSwitchDesktop ) , 2000 , false )
{
	// After the Boot up finishes,
	// see if this is a new Setup
	if( _registryController::getSystemRegistry().readIndex( "_global_" , "firstTimeUse" ).length() ){
		_guiController::changeState( _guiState::setup );
		return;
	}
	
	_guiController::disableKeyboardMagnif(); // Prevent Magnifying of the keyboard focus
	_guiController::setHost<_startupScreen>();
	_guiController::enableKeyboard();
	
	// Make list of all users
	_direntry userDir = _direntry( "%USERS%" );
	userDir.rewindChildren();
	
	// Read Children of directory
	for( _literal str; userDir.readChild( str ) != false ; )
		this->users.push_back( str );
	
	// Copy users and their logos
	int userHeight = 24;
	
	loginAnim.setSetter( make_callback( this , &_guiLogin::loginAnimSetter ) );
	loginAnim.setEasing( _animation::_sinus::easeInOut );
	loginAnim.setToValue( 78 );
	
	// Position on the screen
	_coord userX = 135;
	_coord userY = ( SCREEN_HEIGHT >> 1 ) - ( userHeight >> 2 );
	_coord curY = 0;
	
	userY -= this->users.size() * userHeight >> 1;
	
	// Create a _userWrapper for each user!
	for( const string& usr : users )
	{
		_userWrapper* wrapper = new _userWrapper( userX , userY + curY , usr , _style::storeHandle( this ) );
		
		// Register Eventhandler to listen if one of the userWrapper logs in
		wrapper->setInternalEventHandler( onMouseClick , make_callback( this , &_guiLogin::userLoginHandler ) );
		
		// Add it
		_guiController::getHost()->addChild( wrapper );
		this->userWrapper.push_back( move(wrapper) );
		
		curY += userHeight; // Increase Y-Coordinate
	}
	
	//! Separator
	_length sepHeight = 148;
	_bitmap separator = _bitmap( 1 , 148 );
	separator.drawVerticalGradient( 0 , 0 			   , 1 , sepHeight >> 1 , _color::fromHex( 0x5A7EDC ) , _color::fromRGB( 21 , 24 , 31 ) );
	separator.drawVerticalGradient( 0 , sepHeight >> 1 , 1 , sepHeight >> 1 , _color::fromRGB( 21 , 24 , 31 ) , _color::fromHex( 0x5A7EDC ) );
	
	this->separator = new _imageGadget( 126 , 20 , separator );
	
	_guiController::getHost()->addChild( this->separator );
}



_guiLogin::~_guiLogin()
{
	_guiController::enableKeyboardMagnif();
	
	delete this->welcome;
	delete this->welcomeBg;
	delete this->separator;
}