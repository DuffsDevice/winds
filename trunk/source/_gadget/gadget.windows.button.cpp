#include "_gadget/gadget.windows.button.h"
#include "_type/type.system.h"

//! Graphics
#include "_resource/BMP_StartButton.h"

_bitmap* _windowsStartButton::startButton = new BMP_StartButton();
_bitmap* _windowsStartButton::startButtonPressed = new BMP_StartButtonPressed();

_callbackReturn _windowsStartButton::mouseHandler( _event event ){
	
	// Receive Gadget
	_windowsStartButton* that = event.getGadget<_windowsStartButton>();
	
	that->startMenu->toggle( that->getAbsoluteX() , that->getAbsoluteY() );
	
	return _button::mouseHandler( event );

}

_callbackReturn _windowsStartButton::refreshHandler( _event event )
{
	static string sBT = _system::_runtimeAttributes_->user->getStrAttr( "startButtonText" );
	
	// Receive Gadget
	_windowsStartButton* that = event.getGadget<_windowsStartButton>();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.hasClippingRects() )
		bP.addClippingRects( event.getDamagedRects().toRelative( that->getAbsoluteX() , that->getAbsoluteY() ) );
	else
		bP.normalizeClippingRects();
	
	if( that->isPressed() || that->startMenu->isOpened() )
		bP.copy( 0 , 0 , that->startButtonPressed );
	else
		bP.copy( 0 , 0 , that->startButton );
	
	// "Start"-Text
	bP.drawString( 12 , 2 , _system::getFont() , sBT , _system::_runtimeAttributes_->user->sBTC );
	
	return use_default;
}

_windowsStartButton::_windowsStartButton( _coord x , _coord y , _style style ) :
	_button( 38 , 10 , x , y , "" , style )
	, startMenu( new _startMenu( this ) )
{
	this->registerEventHandler( onAction , &_windowsStartButton::mouseHandler );
	this->registerEventHandler( refresh , &_windowsStartButton::refreshHandler );
	
	this->refreshBitmap();
}
