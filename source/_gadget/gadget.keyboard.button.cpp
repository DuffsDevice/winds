#include "_gadget/gadget.keyboard.button.h"
#include "_type/type.system.h"

#include "click_bin.h"
#include "_type/type.sound.h"

//_sound* sound = new _sound( click_bin , click_bin_size , _soundEncoding::mono16 , 44100 );

_callbackReturn _keyboardButton::mouseHandler( _event event )
{
	// Receive Gadget
	_keyboardButton* that = event.getGadget<_keyboardButton>();
	
	_event ev;
	
	if( event.getType() == mouseDown )
		ev = _event( keyDown );
	else if( event.getType() == mouseUp )
		ev = _event( keyUp );
	else if( event.getType() == mouseClick )
		ev = _event( keyClick );
	else if( event.getType() == mouseRepeat )
		ev = _event( keyRepeat );
	else
		ev = _event( _none_ );
	
	//if( event.getType() == mouseClick )
	//	sound->play();
	
	// Set Key-code(s)
	ev.setKeyCode( that->key );
	ev.setCurrentKeys( _system::getCurrentKeys() );
	
	if( that->parent != nullptr )
		that->parent->handleEvent( ev );
	
	return handled;
}

_keyboardButton::_keyboardButton( _key key , _length width , _length height , _coord x , _coord y , string title , _style&& style )
	: _button( width , height , x , y , title , (_style&&)style )
	, key( key )
{	
	this->setInternalEventHandler( mouseDown , _staticCallback( &_keyboardButton::mouseHandler ) );
	this->setInternalEventHandler( mouseUp , _staticCallback( &_keyboardButton::mouseHandler ) );
	this->setInternalEventHandler( mouseClick , _staticCallback( &_keyboardButton::mouseHandler ) );
	this->setInternalEventHandler( mouseRepeat , _staticCallback( &_keyboardButton::mouseHandler ) );
}

_keyboardButton::_keyboardButton( _key key , _coord x , _coord y , string text , _style&& style )
	: _button( x , y , text , (_style&&)style )
	, key( key )
{
	this->setInternalEventHandler( mouseDown , _staticCallback( &_keyboardButton::mouseHandler ) );
	this->setInternalEventHandler( mouseUp , _staticCallback( &_keyboardButton::mouseHandler ) );
	this->setInternalEventHandler( mouseClick , _staticCallback( &_keyboardButton::mouseHandler ) );
	this->setInternalEventHandler( mouseRepeat , _staticCallback( &_keyboardButton::mouseHandler ) );
}
