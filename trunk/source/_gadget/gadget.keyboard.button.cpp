#include "_gadget/gadget.keyboard.button.h"
#include "_type/type.system.h"

//! Graphics
#include "_resource/BMP_StartButton.h"

_callbackReturn _keyboardButton::mouseHandler( _event event )
{
	// Receive Gadget
	_keyboardButton* that = event.getGadget<_keyboardButton>();
	
	if( event.getType() == mouseClick )
	{
		_event ev = _event( keyClick );
		
		// Set Key-code
		ev.setKeyCode( that->key );
		
		if( that->getScreen() != nullptr )
			ev.setCurrentKeyCodes( _system::getCurrentKeys() );
		
		if( that->parent != nullptr )
			that->parent->handleEvent( ev );
	}
	else if( event.getType() == mouseDown )
	{
		_event ev = _event( keyDown );
		
		// Set Key-code
		ev.setKeyCode( that->key );
		
		if( that->getScreen() )
			ev.setCurrentKeyCodes( _system::getCurrentKeys() );
		
		if( that->parent )
			that->parent->handleEvent( ev );
		
		
		// Start Key-Repetition
		if( _system::_runtimeAttributes_->user->kRD )
		{
			_event e = _event( _internal_ );
			e.setHeldTime( 0 );
			that->triggerEvent( _event( _internal_ ) );
		}
		
		return _button::mouseHandler( event );
	}
	else
	{
		if( that->isPressed() ){
			// Trigger Event
			_event e = _event( _internal_ );
			e.setHeldTime( event.getHeldTime() + 1 );
			that->triggerEvent( e );
		}
		if( _system::_runtimeAttributes_->user->kRD && event.getHeldTime() > _system::_runtimeAttributes_->user->kRD && event.getHeldTime() % _system::_runtimeAttributes_->user->kRS == 0 ) 
		{
			_event ev = _event( keyDown );
			
			// Set Key-code
			ev.setKeyCode( that->key );
			
			if( that->getScreen() != nullptr )
				ev.setCurrentKeyCodes( _system::getCurrentKeys() );
			
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
	this->unregisterEventHandler( mouseDoubleClick );
	this->registerEventHandler( mouseClick , new _staticCallback( &_keyboardButton::mouseHandler ) );
	this->registerEventHandler( mouseDown , new _staticCallback( &_keyboardButton::mouseHandler ) );
	this->registerEventHandler( _internal_ , new _staticCallback( &_keyboardButton::mouseHandler ) );
}

_keyboardButton::_keyboardButton( _key key , _length width , _length height , _coord x , _coord y , string title , _style style )
	: _button( width , height , x , y , title , style ) , key( key )
{ this->init(); }

_keyboardButton::_keyboardButton( _key key , _coord x , _coord y , string text , _style style )
	: _button( x , y , text , style ) , key( key )
{ this->init(); }

// ---------------------------- //
//			Start-button		//
// ---------------------------- //

_bitmap _keyboardStartButton::startButton = BMP_StartButton();
_bitmap _keyboardStartButton::startButtonPressed = BMP_StartButtonPressed();

_callbackReturn _keyboardStartButton::mouseHandler( _event event ){
	
	// Receive Gadget
	_keyboardStartButton* that = event.getGadget<_keyboardStartButton>();
	
	if( event.getType() == focus )
		return handled;
	
	that->startMenu->toggle( that->getAbsoluteX() , that->getAbsoluteY() );
	
	return _button::mouseHandler( event );

}

_callbackReturn _keyboardStartButton::refreshHandler( _event event )
{
	static string sBT = _system::_runtimeAttributes_->user->getStrAttr( "startButtonText" );
	
	// Receive Gadget
	_keyboardStartButton* that = event.getGadget<_keyboardStartButton>();
	
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
	
	if( event.getType() == close )
		that->bubbleRefresh();
	
	return use_default;
}

_keyboardStartButton::_keyboardStartButton( _coord x , _coord y , _style style ) :
	_button( 38 , 10 , x , y , "" , style )
	, startMenu( new _startMenu() )
{
	this->registerEventHandler( mouseClick , new _staticCallback( &_keyboardStartButton::mouseHandler ) );
	this->registerEventHandler( refresh , new _staticCallback( &_keyboardStartButton::refreshHandler ) );
	this->registerEventHandler( close , new _staticCallback( &_keyboardStartButton::refreshHandler ) );
	
	this->refreshBitmap();
}
