#include "_gadget/gadget.keyboard.button.h"
#include "_type/type.system.h"

//! Graphics
#include "_graphic/BMP_StartButton.h"

_gadgetEventReturnType _keyboardButton::mouseHandler( _gadgetEvent event )
{
	// Receive Gadget
	_keyboardButton* that = (_keyboardButton*)event.getGadget();
	
	_gadgetEvent ev = _gadgetEvent( keyClick );
	
	// Set Key-code
	ev.getArgs().setKeyCode( that->key );
	
	if( that->getWindows() != nullptr )
		ev.getArgs().setCurrentKeyCodes( _system_->getCurrentKeys() );
	
	if( that->parent != nullptr )
		that->parent->handleEvent( ev );
	
	_button::mouseHandler( event );
	
	return handled;
}

_key _keyboardButton::getKey(){ return this->key; }

void _keyboardButton::setKey( _key key ){ this->key = key; }

void _keyboardButton::init()
{
	this->unregisterEventHandler( mouseDoubleClick );
	this->registerEventHandler( mouseClick , &_keyboardButton::mouseHandler );
}

_keyboardButton::_keyboardButton( _key key , _length width , _length height , _coord x , _coord y , string title , _gadgetStyle style )
	: _button( width , height , x , y , title , style ) , key( key )
{ this->init(); }

_keyboardButton::_keyboardButton( _key key , _coord x , _coord y , string text , _gadgetStyle style )
	: _button( x , y , text , style ) , key( key )
{ this->init(); }

// ---------------------------- //
//			Start-button		//
// ---------------------------- //

_bitmap* _keyboardStartButton::startButton = new BMP_StartButton();

_gadgetEventReturnType _keyboardStartButton::mouseHandler( _gadgetEvent event ){
	
	// Receive Gadget
	_keyboardStartButton* that = (_keyboardStartButton*)event.getGadget();
	
	that->startMenu->toggle( that->getAbsoluteX() , that->getAbsoluteY() );
	
	return handled;

}

_gadgetEventReturnType _keyboardStartButton::refreshHandler( _gadgetEvent event )
{
	// Receive Gadget
	_keyboardStartButton* that = (_keyboardStartButton*)event.getGadget();
	
	_bitmapPort bP = that->getBitmapPort();
	
	if( event.getArgs().hasClippingRects() )
		bP.addClippingRects( event.getArgs().getDamagedRects().toRelative( that->getAbsoluteDimensions() ) );
	else
		bP.resetClippingRects();
	
	bP.copy( 0 , 0 , that->startButton );
	
	// "Start"-Text
	bP.drawString( 12 , 2 , _system_->_runtimeAttributes_->defaultFont , _system_->_runtimeAttributes_->startButtonText , RGB( 30 , 30 , 30 ) );
	
	return handled;
}

_keyboardStartButton::_keyboardStartButton( _coord x , _coord y , _gadgetStyle style ) :
	_gadget( 38 , 10 , x , y ,  style )
	, startMenu( new _startMenu() )
{
	this->unregisterEventHandler( mouseDoubleClick );
	this->registerEventHandler( mouseClick , &_keyboardStartButton::mouseHandler );
	this->registerEventHandler( refresh , &_keyboardStartButton::refreshHandler );
	
	this->refreshBitmap();
}
