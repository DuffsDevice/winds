#include "_gadget/gadget.windows.button.h"
#include "_type/type.system.h"

//! Graphics
#include "_resource/BMP_StartButton.h"

_bitmap* _windowsStartButton::startButton = new BMP_StartButton();
_bitmap* _windowsStartButton::startButtonPressed = new BMP_StartButtonPressed();

_gadgetEventReturnType _windowsStartButton::mouseHandler( _gadgetEvent event ){
	
	// Receive Gadget
	_windowsStartButton* that = (_windowsStartButton*)event.getGadget();
	
	that->startMenu->toggle( that->getAbsoluteX() , that->getAbsoluteY() );
	
	return _button::mouseHandler( event );

}

_gadgetEventReturnType _windowsStartButton::refreshHandler( _gadgetEvent event )
{
	static string sBT = _system_->_runtimeAttributes_->user->getStrAttr( "startButtonText" );
	// Receive Gadget
	_windowsStartButton* that = (_windowsStartButton*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	if( that->isPressed() || that->startMenu->isOpened() )
		bP.copy( 0 , 0 , that->startButtonPressed );
	else
		bP.copy( 0 , 0 , that->startButton );
	
	// "Start"-Text
	bP.drawString( 12 , 2 , _system_->_runtimeAttributes_->defaultFont , sBT , _system_->_runtimeAttributes_->user->getIntAttr( "startButtonTextColor" ) );
	
	if( event.getType() == "dialogClose" )
		that->bubbleRefresh();
	
	return use_default;
}

_windowsStartButton::_windowsStartButton( _coord x , _coord y , _gadgetStyle style ) :
	_button( 38 , 10 , x , y , "" , style )
	, startMenu( new _startMenu( this ) )
{
	this->registerEventHandler( "mouseClick" , &_windowsStartButton::mouseHandler );
	this->registerEventHandler( "refresh" , &_windowsStartButton::refreshHandler );
	this->registerEventHandler( "dialogClose" , &_windowsStartButton::refreshHandler );
	
	this->refreshBitmap();
}
