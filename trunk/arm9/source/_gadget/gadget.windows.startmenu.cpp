#include "_gadget/gadget.windows.startmenu.h"
#include "_gadget/gadget.image.h"
#include "_gadget/gadget.fileview.h"
#include "_gadget/gadget.button.action.h"
#include "_type/type.system.h"

#define CONST_BOTTOM_BAR_HEIGHT 12
#define CONST_TOP_BAR_HEIGHT 16

_callbackReturn _startMenu::clickHandler( _event event ){
	if( event == onMouseClick ){
		event.getGadget<_fileObject>()->execute();
		event.getGadget()->blur();
		this->shelve();
	}
	return use_internal;
}

_callbackReturn _startMenu::refreshHandler( _event event ){
	
	_startMenu* that = event.getGadget<_startMenu>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myW = that->getWidth();
	_length myH = that->getHeight();
	
	// Top Border and Header
	bP.drawVerticalGradient( 1 , 1 , myW - 2 , CONST_TOP_BAR_HEIGHT , _color::fromRGB8( 33 , 119 , 214 ) , _color::fromRGB8( 59 , 155 , 238 ) );
	bP.drawHorizontalLine( 1 , 0 , myW - 2 , _color::fromRGB8( 117 , 173 , 233 ) );
	
	// Line Between Header & Body
	bP.drawHorizontalGradient( 1 , CONST_TOP_BAR_HEIGHT + 1 , ( myW - 1 ) >> 1 , 1 , _color::fromRGB8( 50 , 120 , 215 ) , _color::fromRGB8( 79 , 154 , 228 ) );
	bP.drawHorizontalGradient( ( myW + 1 ) >> 1 , CONST_TOP_BAR_HEIGHT + 1 , ( myW - 1 ) >> 1 , 1 , _color::fromRGB8( 79 , 154 , 228 ) , _color::fromRGB8( 50 , 125 , 218 ) );
	
	// Borders Left + Right
	bP.drawVerticalGradient( myW - 1 , 0 , 1 , myH , _color::fromRGB8( 16 , 90 , 189 ) , _color::fromRGB8( 101 , 151 , 220 ) );
	bP.drawVerticalGradient( 0 , 0 , 1 , myH  , _color::fromRGB8( 101 , 151 , 220 ) , _color::fromRGB8( 150 , 177 , 227 ) );
	
	// Line Between Footer & Body
	bP.drawHorizontalLine( 1 , myH - CONST_BOTTOM_BAR_HEIGHT - 2 , myW - 2 , _color::fromRGB8( 115 , 170 , 232 ) );
	bP.drawHorizontalLine( 1 , myH - CONST_BOTTOM_BAR_HEIGHT - 1 , myW - 2 , _color::fromRGB8( 60 , 141 , 229 ) );
	
	// Footer
	bP.drawVerticalGradient( 1 , myH - CONST_BOTTOM_BAR_HEIGHT , myW - 2 , 13 , _color::fromRGB8( 83 , 154 , 233 ) , _color::fromRGB8( 36 , 119 , 210 ) );
	
	// Right Side
	bP.drawFilledRect( myW >> 1 , CONST_TOP_BAR_HEIGHT + 2 , ( myW - 1 ) >> 1 , myH - CONST_TOP_BAR_HEIGHT - CONST_BOTTOM_BAR_HEIGHT - 4 , _color::fromRGB8( 230 , 233 , 250 ) );
	
	// Vertical Separator
	bP.drawVerticalLine( ( myW >> 1 ) + 1 , CONST_TOP_BAR_HEIGHT + 2 , myH - CONST_TOP_BAR_HEIGHT - CONST_BOTTOM_BAR_HEIGHT - 4 , _color::fromRGB8( 205 , 226 , 249 ) );
	
	return use_default;
}

_startMenu::_startMenu( _style&& style ) :
	_popup( 110 , 110 , nullptr , (_style&&)style )
{
	_length halfWidth = this->getWidth() >> 1;
	
	// Left Side
	_fileView* view = new _fileView( 1 , CONST_TOP_BAR_HEIGHT + 2
			, ( this->getWidth() - 2 ) >> 1 , this->getHeight() - CONST_TOP_BAR_HEIGHT - CONST_BOTTOM_BAR_HEIGHT - 4
			, "%CURRENT_USER%/jumplist/" , _fileViewType::list , false , { "exe" , "lua" , "lnk" }
		);
	
	view->setEventHandler( make_callback( this , &_startMenu::clickHandler ) );
	view->setScrollTypeX( _scrollType::prevent );
	view->setScrollTypeY( _scrollType::prevent );
	this->addChild( view );
	
	// User-Image
	this->addChild( new _imageGadget( 3 , 2 , _system::getUser().getLogo() ) );
	
	
	// Right Side
	//this->addChild( new _fileView( ( this->getWidth() - 2 ) >> 1 , this->getHeight() - CONST_TOP_BAR_HEIGHT - CONST_BOTTOM_BAR_HEIGHT - 4 , ( this->getWidth() + 4 ) >> 1 , CONST_TOP_BAR_HEIGHT + 2 , "%WINDIR%/startmenu/" , _fileViewType::liste , _scrollType::prevent , _scrollType::prevent ) );
	
	_fileObject* credits = new _fileObject( halfWidth + 3 , CONST_TOP_BAR_HEIGHT + 3 , halfWidth - 5 , ignore , "%SYSTEM%/Credits.exe" , _fileViewType::list );
	credits->setUserEventHandler( onMouseClick , make_callback( this , &_startMenu::clickHandler ) );
	this->addChild( credits );// Credentials
	
	// Y-Coordinate for all information that goes in the bottom layer
	_coord buttonY = this->getHeight() - 11;
	
	// Shutdown button & label
	_label* shutdownLabel = new _label( halfWidth + 13 , buttonY , ignore , 10 , "Turn off" );
	shutdownLabel->setColor( _color::white );
	shutdownLabel->setVAlign( _valign::middle );
	this->addChild( new _actionButton( halfWidth + 2 , buttonY , _actionButtonType::shutdown ) );
	this->addChild( shutdownLabel );
	
	// Log Off button & label
	_label* logOffLabel = new _label( 14 , buttonY , ignore , 10 , "Log off" );
	logOffLabel->setColor( _color::white );
	logOffLabel->setVAlign( _valign::middle );
	this->addChild( new _actionButton( 3 , buttonY , _actionButtonType::logoff ) );
	this->addChild( logOffLabel );
	
	// Username
	_label* usrName = new _label( 20 , 2 , this->getWidth() - 20 - 2 , 14 , _system::getUser().getUsername() );
	usrName->setVAlign( _valign::middle );
	usrName->setAlign( _align::left );
	usrName->setColor( _color::white );
	usrName->setFont( _system::getFont("System10") );
	this->addChild( usrName );
	
	// Registering Event Handlers
	this->setInternalEventHandler( onDraw , make_callback( &_startMenu::refreshHandler ) );
	
	// Refresh...
	this->redraw();
}

_startMenu::~_startMenu(){
	this->removeChildren( true );
}
