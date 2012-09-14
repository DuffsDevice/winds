#include "_gadget/gadget.windows.startMenu.h"
#include "_gadget/gadget.fileview.h"
#include "_type/type.system.h"

#define CONST_BOTTOM_BAR_HEIGHT 11
#define CONST_TOP_BAR_HEIGHT 16

 _gadgetEventReturnType _startMenu::refreshHandler( _gadgetEvent event ){
	
	_startMenu* that = event.getGadget<_startMenu>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.resetClippingRects();
	
	_length myW = that->dimensions.width;
	_length myH = that->dimensions.height;
	
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

_startMenu::_startMenu( _gadget* owner , _gadgetStyle style ) :
	_contextMenu( 110 , 110 , owner , style )
{
	// Reset Bitmap
	this->bitmap->reset( COLOR_WHITE );
	
	// Left Side
	this->addChild( new _fileview( ( this->dimensions.width - 2 ) >> 1 , this->dimensions.height - CONST_TOP_BAR_HEIGHT - CONST_BOTTOM_BAR_HEIGHT - 4 , 1 , CONST_TOP_BAR_HEIGHT + 2 , "%WINDIR%/jumplist/" , _fileviewType::liste , _scrollType::prevent , _scrollType::prevent ) );
	
	// Right Side
	//this->addChild( new _fileview( ( this->dimensions.width - 2 ) >> 1 , this->dimensions.height - CONST_TOP_BAR_HEIGHT - CONST_BOTTOM_BAR_HEIGHT - 4 , ( this->dimensions.width + 4 ) >> 1 , CONST_TOP_BAR_HEIGHT + 2 , "%WINDIR%/startmenu/" , _fileviewType::liste , _scrollType::prevent , _scrollType::prevent ) );
	
	// User-Image
	this->addChild( new _imagegadget( 3 , 2 , _system_->_runtimeAttributes_->user->userLogo ) );
	
	// Username
	_label* usrName = new _label( this->dimensions.width - 19 - 2 , 14 , 19 , 2 , _system_->_runtimeAttributes_->user->getUsername() );
	usrName->setVAlign( _valign::middle );
	usrName->setColor( COLOR_WHITE );
	this->addChild( usrName );
	
	// Registering Event Handlers
	this->registerEventHandler( "refresh" , _startMenu::refreshHandler );
	
	// Refresh...
	this->refreshBitmap();
}