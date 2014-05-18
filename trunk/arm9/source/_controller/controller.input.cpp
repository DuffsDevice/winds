#include "_controller/controller.input.h"
#include "_controller/controller.gui.h"
#include "_controller/controller.registry.h"
#include "_type/type.event.h"
#include "_type/type.key.h"
#include "_type/type.screen.gadget.h"
#include "_screen/screen.desktop.h"

extern "C"{
	#include <nds/touch.h>
}

bool _inputController::init(){
	for( _u8 i = 0 ; i < 16 ; i++ )
		_inputController::heldCycles[i] = 0;
	return true;
}


void _inputController::frame()
{
	// Refresh Input
	scanKeys();
	
	touchPosition t;
	touchRead( &t );

	// Temp...
	_event				event = _event().setCurrentKeys( _hardwareKeyPattern::getCurrentKeys() );
	_gadgetScreen*		currentHost = _guiController::getHost();
	_gadget*			currentFocus = _guiController::getCurrentFocus();
	
	_hardwareKeyPattern	keys = _hardwareKeyPattern::getCurrentInputs();
	_hardwareKeyPattern	newKeys = _hardwareKeyPattern::getNewInputs();
	_keyboardScreen*	keyboard = _guiController::getKeyboard();
	
	// Forward the touch to the current Host-Screen, if, and only if the touch was not caught by the keyboard
	if( !keyboard || !keyboard->processTouch( keys.isKeySet( _key::touch ) , t ) )
		currentHost && currentHost->processTouch( keys.isKeySet( _key::touch ) , t );
	
	if( keyboard )
	{
		if( newKeys.isKeySet( _key::select ) )
		{
			if( keyboard->isOpened() )
				keyboard->close();
			else
				keyboard->open();
		}
		if( keyboard->isOpened() )
		{
			// Don't forward L&R-Keys to the Host, if the Keyboard is opened
			keys.remove( _key::l );
			keys.remove( _key::r );
			
			if( newKeys.isKeySet( _key::l ) )
				keyboard->setShift( !keyboard->isShift() );
			if( newKeys.isKeySet( _key::r ) )
				keyboard->setCaps( !keyboard->isCaps() );
		}
	}
	
	if( newKeys.isKeySet( _key::windows ) && currentHost->getScreenType() == _gadgetScreenType::desktop ){
		_desktopScreen* win = (_desktopScreen*)currentHost;
		win->toggleStartMenu();
	}

	if( !currentFocus && !_guiController::getHost() )
		return;
	
	/**
	 * Just Handle the Buttons, not the Pen!
	 */
	for( _u8 i = 0 ; i < 16 ; ++i )
	{
		_key currentKey = _hardwareKeyPattern::libndsBit2key(i);
		
		//! Again: We do not handle Pen (as well as the lid)
		if( currentKey == _key::touch || currentKey == _key::lid )
			continue;
		
		event.setKeyCode( currentKey );
		
		// Held down
		if( keys.isKeySet( currentKey ) )
		{
			if( heldCycles[i] == 0 )
			{
				if( currentFocus )
					currentFocus->triggerEvent( event.setType( onKeyDown ) );
				else
					currentHost->triggerEvent( event.setType( onKeyDown ) );
			}
			else if( _guiController::getKeyRepetitionDelay() && heldCycles[i] > _guiController::getKeyRepetitionDelay() && heldCycles[i] % _guiController::getKeyRepetitionPause() == 0 )
			{
				// Set the Args and Trigger the Event
				if( currentFocus )
					currentFocus->triggerEvent( event.setType( onKeyRepeat ) );
				else
					currentHost->triggerEvent( event.setType( onKeyRepeat ) );
			}
			
			// Increase Cycles
			heldCycles[i]++;
		}
		// Not held down and was pressed previously
		else if( heldCycles[i] > 0 )
		{
			// Set the Args
			event.setType( onKeyUp );
			
			// Trigger the Event
			if( currentFocus )
				currentFocus->triggerEvent( event );
			else
				currentHost->triggerEvent( event );
			
			
			// If keyup is fast enough, trigger keyClick
			if( heldCycles[i] < _guiController::getMaxClickPeriod() )
			{
				if( currentFocus )
					currentFocus->triggerEvent( event.setType( onKeyClick ) );
				else
					currentHost->triggerEvent( event.setType( onKeyClick ) );
			}
			
			// Reset Cycles
			heldCycles[i] = 0;
		}
	}
}


_u32 _inputController::heldCycles[16];