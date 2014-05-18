#include "_screen/screen.keyboard.button.h"
#include "_screen/screen.keyboard.h"
#include "_screen/screen.desktop.startMenu.h"
#include "_type/type.windows.soundbank.h"
#include "_type/type.key.h"

_callbackReturn _keyboardScreenButton::mouseHandler( _event event )
{
	// Receive Gadget
	_keyboardScreenButton* that = event.getGadget<_keyboardScreenButton>();
	
	_event ev( _none_ );
	switch( event.getType() )
	{
		case onMouseClick:	ev.setType( onKeyClick ); break;
		case onMouseDown:
			ev.setType( onKeyDown );
			_windowsSoundBank::play( _windowsSound::keyClick );
			break;
	    case onMouseUp:		ev.setType( onKeyUp ); break;
		case onMouseRepeat:	ev.setType( onKeyRepeat ); break;
		default: break;
	}
	
	// Set Key-code(s)
	ev.setKeyCode( that->key );
	ev.setCurrentKeys( _hardwareKeyPattern::getCurrentKeys() );
	
	if( that->getParent() )
		that->getParent()->handleEvent( ev );
	
	return handled;
}

_keyboardScreenButton::_keyboardScreenButton( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _key key , string title , _style&& style ) :
	_button( x , y , width , height , title , (_style&&)style )
	, key( key )
{	
	this->setInternalEventHandler( onMouseDown , make_callback( &_keyboardScreenButton::mouseHandler ) );
	this->setInternalEventHandler( onMouseUp , make_callback( &_keyboardScreenButton::mouseHandler ) );
	this->setInternalEventHandler( onMouseClick , make_callback( &_keyboardScreenButton::mouseHandler ) );
	this->setInternalEventHandler( onMouseRepeat , make_callback( &_keyboardScreenButton::mouseHandler ) );
}
