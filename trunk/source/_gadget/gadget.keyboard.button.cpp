#include "_gadget/gadget.keyboard.button.h"
#include "_type/type.system.h"

//! Graphics
#include "_graphic/BMP_StartButton.h"

_gadgetEventReturnType _keyboardButton::mouseHandler( _gadgetEvent event )
{
	// Receive Gadget
	_keyboardButton* that = (_keyboardButton*)event.getGadget();
	
	if( event.getType() == "mouseClick" )
	{
		_gadgetEvent ev = _gadgetEvent( "keyClick" );
		
		// Set Key-code
		ev.getArgs().setKeyCode( that->key );
		
		if( that->getWindows() != nullptr )
			ev.getArgs().setCurrentKeyCodes( _system_->getCurrentKeys() );
		
		if( that->parent != nullptr )
			that->parent->handleEvent( ev );
	}
	else if( event.getType() == "mouseDown" )
	{
		_gadgetEvent ev = _gadgetEvent( "keyDown" );
		
		// Set Key-code
		ev.getArgs().setKeyCode( that->key );
		
		if( that->getWindows() )
			ev.getArgs().setCurrentKeyCodes( _system_->getCurrentKeys() );
		
		if( that->parent )
			that->parent->handleEvent( ev );
		
		
		// Start Key-Repetition
		if( _system::_runtimeAttributes_->user->getIntAttr( "keyRepetitionDelay" ) )
		{
			_gadgetEvent e = _gadgetEvent( "repeater" );
			e.getArgs().setHeldTime( 0 );
			that->triggerEvent( _gadgetEvent( "repeater" ) );
		}
		
		return _button::mouseHandler( event );
	}
	else
	{
		if( that->isPressed() ){
			// Trigger Event
			_gadgetEvent e = _gadgetEvent( "repeater" );
			e.getArgs().setHeldTime( event.getArgs().getHeldTime() + 1 );
			that->triggerEvent( e );
		}
		if( _system::_runtimeAttributes_->user->getIntAttr( "keyRepetitionDelay" ) && event.getArgs().getHeldTime() > _system::_runtimeAttributes_->user->getIntAttr( "keyRepetitionDelay" ) && event.getArgs().getHeldTime() % _system::_runtimeAttributes_->user->getIntAttr( "keyRepetitionSpeed" ) == 0 ) 
		{
			_gadgetEvent ev = _gadgetEvent( _gadgetEvent( "keyDown" ) );
			
			// Set Key-code
			ev.getArgs().setKeyCode( that->key );
			
			if( that->getWindows() != nullptr )
				ev.getArgs().setCurrentKeyCodes( _system_->getCurrentKeys() );
			
			if( that->parent != nullptr )
				that->parent->handleEvent( ev );
		}
	}
	return handled;
}

_key _keyboardButton::getKey(){ return this->key; }

void _keyboardButton::setKey( _key key ){ this->key = key; }

void _keyboardButton::init()
{
	this->unregisterEventHandler( "mouseDoubleClick" );
	this->registerEventHandler( "mouseClick" , &_keyboardButton::mouseHandler );
	this->registerEventHandler( "mouseDown" , &_keyboardButton::mouseHandler );
	this->registerEventHandler( "repeater" , &_keyboardButton::mouseHandler );
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
_bitmap* _keyboardStartButton::startButtonPressed = new BMP_StartButtonPressed();

_gadgetEventReturnType _keyboardStartButton::mouseHandler( _gadgetEvent event ){
	
	// Receive Gadget
	_keyboardStartButton* that = (_keyboardStartButton*)event.getGadget();
	
	that->startMenu->toggle( that->getAbsoluteX() , that->getAbsoluteY() );
	
	return _button::mouseHandler( event );

}

_gadgetEventReturnType _keyboardStartButton::refreshHandler( _gadgetEvent event )
{
	static string sBT = _system_->_runtimeAttributes_->user->getStrAttr( "startButtonText" );
	// Receive Gadget
	_keyboardStartButton* that = (_keyboardStartButton*)event.getGadget();
	
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
	bP.drawString( 12 , 2 , _system_->_runtimeAttributes_->defaultFont , sBT , RGB( 30 , 30 , 30 ) );
	
	if( event.getType() == "dialogClose" )
		that->bubbleRefresh();
	
	return use_default;
}

_keyboardStartButton::_keyboardStartButton( _coord x , _coord y , _gadgetStyle style ) :
	_button( 38 , 10 , x , y , "" , style )
	, startMenu( new _startMenu( this ) )
{
	this->registerEventHandler( "mouseClick" , &_keyboardStartButton::mouseHandler );
	this->registerEventHandler( "refresh" , &_keyboardStartButton::refreshHandler );
	this->registerEventHandler( "dialogClose" , &_keyboardStartButton::refreshHandler );
	
	this->refreshBitmap();
}
