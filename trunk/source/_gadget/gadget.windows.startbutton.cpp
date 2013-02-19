#include "_gadget/gadget.windows.startbutton.h"
#include "_type/type.system.h"

//! Graphics
#include "_resource/BMP_StartButton.h"


_callbackReturn _windowsStartButton::mouseHandler( _event event ){
	
	this->startMenu->toggle( this->getAbsoluteX() , this->getAbsoluteY() );
	
	// Let the button handle the rest of the mouseDown event
	return handled;
}


_callbackReturn _windowsStartButton::refreshHandler( _event event )
{
	static string sBT = _system::_runtimeAttributes_->user->getStrAttr( "startButtonText" );
	
	// Receive Gadget
	_windowsStartButton* that = event.getGadget<_windowsStartButton>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsolutePosition() ) );
	else
		bP.normalizeClippingRects();
	
	if( that->isPressed() || that->startMenu->isOpened() )
		bP.copy( 0 , 0 , BMP_StartButtonPressed() );
	else
		bP.copy( 0 , 0 , BMP_StartButton() );
	
	// "Start"-Text
	bP.drawString( 12 , 1 , _system::getFont() , sBT , _system::_runtimeAttributes_->user->sBTC );
	
	return use_default;
}


_windowsStartButton::_windowsStartButton( _coord x , _coord y , _style style ) :
	_button( 38 , 10 , x , y , "" , style | _styleAttr::canNotReceiveFocus | _styleAttr::canNotTakeFocus )
	, startMenu( new _startMenu() )
{
	this->registerEventHandler( mouseClick , new _classCallback( this , &_windowsStartButton::mouseHandler ) );
	this->registerEventHandler( refresh , new _staticCallback( &_windowsStartButton::refreshHandler ) );
	this->startMenu->registerEventHandler( onOpen , new _gadget::eventForwardRefreshGadget( this ) );
	this->startMenu->registerEventHandler( onClose , new _gadget::eventForwardRefreshGadget( this ) );
	
	this->refreshBitmap();
}