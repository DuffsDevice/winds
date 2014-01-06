#include "_scenario/scenario.login.h"
#include "_type/type.system.h"
#include "_type/type.system.controller.h"
#include "_type/type.user.guest.h"


//! Gadgets we need
#include "_gadget/gadget.keyboard.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.screen.startup.h"


_callbackReturn _scLogin::userLoginHandler( _event event )
{
	_userWrapper* that = event.getGadget<_userWrapper>();
	
	// Delete all wrappers but the supplied one
	this->userWrapper.remove_if( [=]( _userWrapper* wrapper ){ if( that != wrapper ){ delete wrapper; return true; } return false; } );
	
	// Blur the currently focused wrapper
	that->blur();
	
	// Prevent it from receiving focus again
	that->setStyle( that->getStyle() | _styleAttr::canNotReceiveFocus );
	
	this->welcome = new _label( 13 , 79 , 105 , 20 , _system::getLocalizedString("lbl_welcome") );
	this->welcomeBg = new _label( 12 , 78 , 105 , 20 , _system::getLocalizedString("lbl_welcome") );
	
	this->welcome->setFont( _system::getFont( "ArialBlack13" ) );
	this->welcomeBg->setFont( _system::getFont( "ArialBlack13" ) );
	this->welcome->setColor( RGB( 2 , 5 , 15 ) );
	this->welcomeBg->setColor( RGB( 30 , 30 , 30 ) );
	this->welcome->setAlign( _align::right );
	this->welcomeBg->setAlign( _align::right );
	
	_system::_gadgetHost_->addChild( welcome );
	_system::_gadgetHost_->addChild( welcomeBg );
	
	this->loginAnim.setFromValue( that->getY() );
	this->loginAnim.start();
	this->loginTmr.start();
	
	return handled;
}


void _scLogin::loginAnimSetter( int val ){
	this->userWrapper.front()->setY( val );
}


void _scLogin::loginSwitchDesktop()
{
	// Log the User in
	_system::switchUser( _user( *this->userWrapper.front()->user ) );
	
	_systemController::changeState( _systemState::desktop );
}


_scLogin::_scLogin() :
	loginAnim( 0 , 0 , 1000 )
	, loginTmr( make_callback( this , &_scLogin::loginSwitchDesktop ) , 2000 , false )
{
	// After the Boot up finishes,
	// see if this is a new Setup
	if( _system::_registry_->readIndex( "_global_" , "firstTimeUse" ).length() )
	{
		_systemController::changeState( _systemState::setup );
		return;
	}
	
	_system::_rtA_->disableKeyboardMagnif(); // Prevent Magnifying of the keyboard focus
	
	_system::_gadgetHost_ = new _startupScreen( _system::_bgIdBack_ );
	_system::_keyboard_ = new _keyboard( _system::_bgIdFront_ , _system::_gadgetHost_ , _system::_topScreen_ );
	
	//! Make list of all users
	_direntry userDir = _direntry( "%USERS%" );
	
	userDir.rewindChildren();
	
	//! Read Children of directory
	for( _literal str; userDir.readChild( str ) != false ; )
		this->users.push_back( new _user( str ) );
	
	//! Copy users and their logos
	int userHeight = 24;
	
	loginAnim.setSetter( make_callback( this , &_scLogin::loginAnimSetter ) );
	loginAnim.setEasing( _animation::_sinus::easeInOut );
	loginAnim.setToValue( 78 );
	
	//! Position on the screen
	_coord userX = 135;
	_coord userY = ( SCREEN_HEIGHT >> 1 ) - ( userHeight >> 2 );
	_coord curY = 0;
	
	userY -= this->users.size() * userHeight >> 1;
	
	//! Create a _userWrapper for each user!
	for( _user* usr : users )
	{
		_userWrapper* img = new _userWrapper( userX , userY + curY , usr , _style::storeHandle( this ) );
		
		// Register Eventhandler to listen if one of the userWrapper logs in
		img->setInternalEventHandler( onMouseClick , make_callback( this , &_scLogin::userLoginHandler ) );
		
		// Add it
		_system::_gadgetHost_->addChild( img );
		this->userWrapper.push_back( img );
		
		curY += userHeight; // Increase Y-Coordinate
	}
	
	//! Separator
	_length sepHeight = 148;
	_bitmap separator = _bitmap( 1 , 148 );
	separator.drawVerticalGradient( 0 , 0 			   , 1 , sepHeight >> 1 , RGBHEX( 0x5A7EDC ) , RGB( 21 , 24 , 31 ) );
	separator.drawVerticalGradient( 0 , sepHeight >> 1 , 1 , sepHeight >> 1 , RGB( 21 , 24 , 31 ) , RGBHEX( 0x5A7EDC ) );
	
	this->separator = new _imageGadget( 126 , 20 , separator );
	
	_system::_gadgetHost_->addChild( this->separator );
}



_scLogin::~_scLogin()
{
	// Delet Imagegadgets
	for( auto& entry : this->users )
		delete entry;
	
	// Delet users
	for( auto& entry : this->userWrapper )
		delete entry;
	
	_system::_rtA_->enableKeyboardMagnif();
	
	delete this->welcome;
	delete this->welcomeBg;
	delete this->separator;
	
}