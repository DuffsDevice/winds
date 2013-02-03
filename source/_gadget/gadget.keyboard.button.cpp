#include "_gadget/gadget.keyboard.button.h"
#include "_type/type.system.h"

_callbackReturn _keyboardButton::mouseHandler( _event event )
{
	// Receive Gadget
	_keyboardButton* that = event.getGadget<_keyboardButton>();
	
	_event ev = _event( keyClick );
	
	// Set Key-code
	ev.setKeyCode( that->key );
	
	if( that->getScreen() != nullptr )
		ev.setCurrentKeyCodes( _system::getCurrentKeys() );
	
	if( that->parent != nullptr )
		that->parent->handleEvent( ev );
	
	return handled;
}

_key _keyboardButton::getKey(){ return this->key; }

void _keyboardButton::setKey( _key key ){ this->key = key; }

_keyboardButton::_keyboardButton( _key key , _length width , _length height , _coord x , _coord y , string title , _style style )
	: _button( width , height , x , y , title , style ) , key( key )
{	
	this->registerEventHandler( onAction , new _staticCallback( &_keyboardButton::mouseHandler ) );
}

_keyboardButton::_keyboardButton( _key key , _coord x , _coord y , string text , _style style )
	: _button( x , y , text , style ) , key( key )
{
	this->registerEventHandler( onAction , new _staticCallback( &_keyboardButton::mouseHandler ) );
}
