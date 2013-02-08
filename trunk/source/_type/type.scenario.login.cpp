#include "_type/type.scenario.login.h"
#include "_type/type.system.h"
#include "_type/type.system.controller.h"

//! Gadgets we need
#include "_gadget/gadget.keyboard.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.startupScreen.h"


_callbackReturn _scLogin::userLoginHandler( _event event )
{
	_userWrapper* that = event.getGadget<_userWrapper>();
	
	// Delete all wrappers but the supplied one
	this->userWrapper.remove_if( [=]( _userWrapper* wrapper ){ if( that != wrapper ){ delete wrapper; return true; } return false; } );
	
	// Blur the currently focused wrapper
	that->blur();
	
	// Prevent it from receiving focus again
	that->setStyle( that->getStyle() | _styleAttr::canNotReceiveFocus );
	
	_label* welcome = new _label( 105 , 20 , 13 , 79 , _system::getLocalizedString("lbl_welcome") );
	_label* welcomeBg = new _label( 105 , 20 , 12 , 78 , _system::getLocalizedString("lbl_welcome") );
	
	welcome->setFont( _system::getFont( "ArialBlack13" ) );
	welcomeBg->setFont( _system::getFont( "ArialBlack13" ) );
	welcome->setColor( RGB( 2 , 5 , 15 ) );
	welcomeBg->setColor( RGB( 30 , 30 , 30 ) );
	welcome->setAlign( _align::right );
	welcomeBg->setAlign( _align::right );
	
	_system::_gadgetHost_->addChild( welcome );
	_system::_gadgetHost_->addChild( welcomeBg );
	
	this->loginAnim.setFromValue( that->getY() );
	this->loginAnim.start();
	
	_system::executeTimer( new _classCallback( this , &_scLogin::loginSwitchDesktop ) , 2000 );
	
	return handled;
}


int _scLogin::loginAnimSetter( int val )
{
	this->userWrapper.front()->setY( val );
	
	return 0;
}


void _scLogin::loginSwitchDesktop()
{
	_system::switchUser( new _user( this->userWrapper.front()->user->getFoldername() ) );
	
	_systemController::changeState( _systemController::_systemState::desktop );
}


_scLogin::_scLogin() :
	loginAnim( 0 , 0 , 1000 )
{
	// After the Boot up finishes,
	// see if this is a new Setup
	//if( _system::_registry_->readIndex( "_global_" , "firstTimeUse" ).length() )
	//{
	//	_systemController::changeState( _systemState::setup );
	//	return;
	//}
	
	_system::_runtimeAttributes_->magnifyKeyboardFocus = false; // Prevent Magnifying of the keyboard focus
	
	//! Clean up
	_system::deleteGadgetHost();
	_system::deleteKeyboard();
	
	_system::_gadgetHost_ = new _startupScreen( _system::_bgIdBack_ );
	_system::_keyboard_ = new _keyboard( _system::_bgIdFront_ , _system::_gadgetHost_ , _system::_topScreen_ );
	
	//! Make list of all users
	_direntry userDir = _direntry( "%USERS%" );
	
	userDir.rewindChildren();
	
	//! Read Children of directory
	for( string str; userDir.readChild( str ) != false ; )
		this->users.push_back( new _user( str ) );
	
	//! Copy users and their logos
	int y = 0;
	int userHeight = 24;
	
	loginAnim.setter( new _classCallback( this , &_scLogin::loginAnimSetter ) );
	loginAnim.setEasing( _animation::_sinus::easeInOut );
	loginAnim.setToValue( 79 );
	
	//! Relative Position to the screen
	int relY = ( SCREEN_HEIGHT - this->users.size() * userHeight ) >> 1;
	relY -= userHeight >> 2;
	
	
	//! Create a _userWrapper for each user!
	for( _user* usr : users )
	{
		_userWrapper* img = new _userWrapper( 135 , y + relY , usr , _style::storeHandle( this ) );
		
		// Register Eventhandler to listen if one of the userWrapper logs in
		img->registerEventHandler( onAction , new _classCallback( this , &_scLogin::userLoginHandler ) );
		
		// Add it
		_system::_gadgetHost_->addChild( img );
		this->userWrapper.push_back( img );
		
		y += userHeight; // Increase Y-Coordinate
	}
	
	//! Separator
	_length sepHeight = 148;
	_bitmap separator = _bitmap( 1 , 148 );
	separator.drawVerticalGradient( 0 , 0 			   , 1 , sepHeight >> 1 , RGBHEX( 0x5A7EDC ) , RGB( 21 , 24 , 31 ) );
	separator.drawVerticalGradient( 0 , sepHeight >> 1 , 1 , sepHeight >> 1 , RGB( 21 , 24 , 31 ) , RGBHEX( 0x5A7EDC ) );
	_system::_gadgetHost_->addChild( new _imagegadget( 126 , 20 , separator ) );
}


_scLogin::~_scLogin()
{
	// Delet Imagegadgets
	for( auto& entry : this->users )
		delete entry;
	
	// Delet users
	for( auto& entry : this->userWrapper )
		delete entry;
	
	_system::_runtimeAttributes_->magnifyKeyboardFocus = true;
}