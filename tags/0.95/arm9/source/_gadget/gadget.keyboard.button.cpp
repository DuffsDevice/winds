#include "_gadget/gadget.keyboard.button.h"
#include "_gadget/gadget.keyboard.h"
#include "_type/type.system.h"

_callbackReturn _keyboardButton::mouseHandler( _event event )
{
	// Receive Gadget
	_keyboardButton* that = event.getGadget<_keyboardButton>();
	
	_event ev( _none_ );
	switch( event.getType() )
	{
		case onMouseClick:	ev.setType( onKeyClick ); break;
		case onMouseDown:
			ev.setType( onKeyDown );
			_keyboard::playClickSound();
			break;
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

_keyboardButton::_keyboardButton( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _key key , string title , _style&& style ) :
	_button( x , y , width , height , title , (_style&&)style )
	, key( key )
{	
	this->setInternalEventHandler( onMouseDown , make_callback( &_keyboardButton::mouseHandler ) );
	this->setInternalEventHandler( onMouseUp , make_callback( &_keyboardButton::mouseHandler ) );
	this->setInternalEventHandler( onMouseClick , make_callback( &_keyboardButton::mouseHandler ) );
	this->setInternalEventHandler( onMouseRepeat , make_callback( &_keyboardButton::mouseHandler ) );
}
