#include "_gadget/gadget.windows.startmenu.h"
#include "_gadget/gadget.imagegadget.h"
#include "_gadget/gadget.fileview.h"
#include "_type/type.system.h"

#define CONST_BOTTOM_BAR_HEIGHT 11
#define CONST_TOP_BAR_HEIGHT 16

 _callbackReturn _startMenu::refreshHandler( _event event ){
	
	_startMenu* that = event.getGadget<_startMenu>();
	
	// Get BitmapPort
	_bitmapPort bP = that->getBitmapPort( event );
	
	_length myW = that->getWidth();
	_length myH = that->getHeight();
	
	// Top Border and Header
	bP.drawVerticalGradient( 1 , 1 , myW - 2 , CONST_TOP_BAR_HEIGHT , RGB255( 33 , 119 , 214 ) , RGB255( 59 , 155 , 238 ) );
	bP.drawHorizontalLine( 1 , 0 , myW - 2 , RGB255( 117 , 173 , 233 ) );
	
	// Line Between Header & Body
	bP.drawHorizontalGradient( 1 , CONST_TOP_BAR_HEIGHT + 1 , ( myW - 1 ) >> 1 , 1 , RGB255( 50 , 120 , 215 ) , RGB255( 79 , 154 , 228 ) );
	bP.drawHorizontalGradient( ( myW + 1 ) >> 1 , CONST_TOP_BAR_HEIGHT + 1 , ( myW - 1 ) >> 1 , 1 , RGB255( 79 , 154 , 228 ) , RGB255( 50 , 125 , 218 ) );
	
	// Borders Left + Right
	bP.drawVerticalGradient( myW - 1 , 0 , 1 , myH , RGB255( 16 , 90 , 189 ) , RGB255( 101 , 151 , 220 ) );
	bP.drawVerticalGradient( 0 , 0 , 1 , myH  , RGB255( 101 , 151 , 220 ) , RGB255( 150 , 177 , 227 ) );
	
	// Line Between Footer & Body
	bP.drawHorizontalLine( 1 , myH - CONST_BOTTOM_BAR_HEIGHT - 2 , myW - 2 , RGB255( 115 , 170 , 232 ) );
	bP.drawHorizontalLine( 1 , myH - CONST_BOTTOM_BAR_HEIGHT - 1 , myW - 2 , RGB255( 60 , 141 , 229 ) );
	
	// Footer
	bP.drawVerticalGradient( 1 , myH - CONST_BOTTOM_BAR_HEIGHT , myW - 2 , 13 , RGB255( 83 , 154 , 233 ) , RGB255( 36 , 119 , 210 ) );
	
	// Right Side
	bP.drawFilledRect( myW >> 1 , CONST_TOP_BAR_HEIGHT + 2 , ( myW - 1 ) >> 1 , myH - CONST_TOP_BAR_HEIGHT - CONST_BOTTOM_BAR_HEIGHT - 4 , RGB255( 230 , 233 , 250 ) );
	
	// Vertical Separator
	bP.drawVerticalLine( ( myW >> 1 ) + 1 , CONST_TOP_BAR_HEIGHT + 2 , myH - CONST_TOP_BAR_HEIGHT - CONST_BOTTOM_BAR_HEIGHT - 4 , RGB255( 205 , 226 , 249 ) );
	
	return use_default;

}

_startMenu::_startMenu( _style&& style ) :
	_popup( 110 , 110 , nullptr , (_style&&)style )
{
	// Left Side
	this->addChild(
		new _fileview( ( this->getWidth() - 2 ) >> 1 , this->getHeight() - CONST_TOP_BAR_HEIGHT - CONST_BOTTOM_BAR_HEIGHT - 4 ,
			1 , CONST_TOP_BAR_HEIGHT + 2 , "%WINDIR%/jumplist/" , _fileviewType::list , _scrollType::prevent , _scrollType::prevent , true
		)
	);
	
	// Right Side
	//this->addChild( new _fileview( ( this->getWidth() - 2 ) >> 1 , this->getHeight() - CONST_TOP_BAR_HEIGHT - CONST_BOTTOM_BAR_HEIGHT - 4 , ( this->getWidth() + 4 ) >> 1 , CONST_TOP_BAR_HEIGHT + 2 , "%WINDIR%/startmenu/" , _fileviewType::liste , _scrollType::prevent , _scrollType::prevent ) );
	
	// User-Image
	this->addChild( new _imagegadget( 3 , 2 , _system::getUser()->getLogo() ) );
	
	// Username
	_label* usrName = new _label( this->getWidth() - 20 - 2 , 14 , 20 , 2 , _system::getUser()->getUsername() );
	usrName->setVAlign( _valign::middle );
	usrName->setAlign( _align::left );
	usrName->setColor( COLOR_WHITE );
	this->addChild( usrName );
	
	// Registering Event Handlers
	this->setInternalEventHandler( onDraw , make_callback( &_startMenu::refreshHandler ) );
	
	// Refresh...
	this->redraw();
}
