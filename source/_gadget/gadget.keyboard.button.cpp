#include "_gadget/gadget.keyboard.button.h"
#include "_type/type.system.h"

#include "click_bin.h"
#include "_type/type.sound.h"

//_sound* sound = new _sound( click_bin , click_bin_size , _soundEncoding::mono16 , 44100 );

_callbackReturn _keyboardButton::mouseHandler( _event event )
{
	// Receive Gadget
	_keyboardButton* that = event.getGadget<_keyboardButton>();
	
	_event ev( _none_ );
	switch( event.getType() )
	{
		case onMouseClick:	ev.setType( onKeyClick ); break;
		case onMouseDown:	ev.setType( onKeyDown ); break;
	    case onMouseUp:		ev.setType( onKeyUp ); break;
		case onMouseRepeat:	ev.setType( onKeyRepeat ); break;
		default: break;
	}
	
	// Set Key-code(s)
	ev.setKeyCode( that->key );
	ev.setCurrentKeys( _system::getCurrentKeys() );
	
	if( that->parent )
		that->parent->handleEvent( ev );
	
	return handled;
}

_keyboardButton::_keyboardButton( _key key , _length width , _length height , _coord x , _coord y , string title , _style&& style ) :
	_button( width , height , x , y , title , (_style&&)style )
	, key( key )
{	
	this->setInternalEventHandler( onMouseDown , make_callback( &_keyboardButton::mouseHandler ) );
	this->setInternalEventHandler( onMouseUp , make_callback( &_keyboardButton::mouseHandler ) );
	this->setInternalEventHandler( onMouseClick , make_callback( &_keyboardButton::mouseHandler ) );
	this->setInternalEventHandler( onMouseRepeat , make_callback( &_keyboardButton::mouseHandler ) );
}

_keyboardButton::_keyboardButton( _key key , _coord x , _coord y , string text , _style&& style ) :
	_button( x , y , text , (_style&&)style )
	, key( key )
{
	this->setInternalEventHandler( onMouseDown , make_callback( &_keyboardButton::mouseHandler ) );
	this->setInternalEventHandler( onMouseUp , make_callback( &_keyboardButton::mouseHandler ) );
	this->setInternalEventHandler( onMouseClick , make_callback( &_keyboardButton::mouseHandler ) );
	this->setInternalEventHandler( onMouseRepeat , make_callback( &_keyboardButton::mouseHandler ) );
}
