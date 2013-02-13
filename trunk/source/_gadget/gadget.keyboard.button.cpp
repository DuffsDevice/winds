#include "_gadget/gadget.keyboard.button.h"
#include "_type/type.system.h"

_callbackReturn _keyboardButton::mouseHandler( _event event )
{
	// Receive Gadget
	_keyboardButton* that = event.getGadget<_keyboardButton>();
	
	_event ev;
	
	if( event.getType() == mouseDown )
		ev.setType( keyDown );
	else if( event.getType() == mouseUp )
		ev.setType( keyUp );
	else if( event.getType() == mouseClick )
		ev.setType( keyClick );
	else if( event.getType() == mouseRepeat )
		ev.setType( keyRepeat );
	
	// Set Key-code(s)
	ev.setKeyCode( that->key );
	ev.setCurrentKeyCodes( _system::getCurrentKeys() );
	
	if( that->parent != nullptr )
		that->parent->handleEvent( ev );
	
	return handled;
}

_key _keyboardButton::getKey(){ return this->key; }

void _keyboardButton::setKey( _key key ){ this->key = key; }

_keyboardButton::_keyboardButton( _key key , _length width , _length height , _coord x , _coord y , string title , _style style )
	: _button( width , height , x , y , title , style | _styleAttr::mouseClickRepeat | _styleAttr::canNotTakeFocus | _styleAttr::canNotReceiveFocus )
	, key( key )
{	
	this->registerEventHandler( mouseDown , new _staticCallback( &_keyboardButton::mouseHandler ) );
	this->registerEventHandler( mouseUp , new _staticCallback( &_keyboardButton::mouseHandler ) );
	this->registerEventHandler( mouseClick , new _staticCallback( &_keyboardButton::mouseHandler ) );
	this->registerEventHandler( mouseRepeat , new _staticCallback( &_keyboardButton::mouseHandler ) );
}

_keyboardButton::_keyboardButton( _key key , _coord x , _coord y , string text , _style style )
	: _button( x , y , text , style | _styleAttr::mouseClickRepeat | _styleAttr::canNotTakeFocus | _styleAttr::canNotReceiveFocus )
	, key( key )
{
	this->registerEventHandler( mouseDown , new _staticCallback( &_keyboardButton::mouseHandler ) );
	this->registerEventHandler( mouseUp , new _staticCallback( &_keyboardButton::mouseHandler ) );
	this->registerEventHandler( mouseClick , new _staticCallback( &_keyboardButton::mouseHandler ) );
	this->registerEventHandler( mouseRepeat , new _staticCallback( &_keyboardButton::mouseHandler ) );
}
