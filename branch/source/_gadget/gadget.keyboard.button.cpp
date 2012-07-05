#include "_gadget/gadget.keyboard.button.h"
#include "_type/type.system.h"

_gadgetEventReturnType _keyboardButton::mouseHandler( _gadgetEvent event )
{
	// Receive Gadget
	_keyboardButton* that = (_keyboardButton*)event.getGadget();
	
	if( event.getType() == "mouseClick" )
	{
		_gadgetEvent ev = _gadgetEvent( "keyClick" );
		
		// Set Key-code
		ev.getArgs().setKeyCode( that->key );
		ev.getArgs().setCurrentKeyCodes( _system_->getCurrentKeys() );
		
		if( that->parent != nullptr )
			that->parent->handleEvent( ev );
	}
	else if( event.getType() == "mouseDown" )
	{
		_gadgetEvent ev = _gadgetEvent( "keyDown" );
		
		// Set Key-code
		ev.getArgs().setKeyCode( that->key );
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
			_gadgetEvent ev = _gadgetEvent( _gadgetEvent( "keyClickRepeat" ) );
			
			// Set Key-code
			ev.getArgs().setKeyCode( that->key );
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