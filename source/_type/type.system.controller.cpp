#include "_type/type.system.h"
#include "_type/type.system.controller.h"

// NDS Interrupts
#include <nds/interrupts.h>

void _systemController::controllerVBL()
{
	if( !dirty )
		return;
	
	irqDisable( IRQ_VBLANK ); // Enter critical Section
	
	// Fade down Screen
	_system::fadeMainScreen( false );
	
	// Set start
	start:
	
	if( _systemController::scenario != nullptr )
	{
		delete _systemController::scenario;
		_systemController::scenario = nullptr;
	}
	
	_systemController::dirty = false;
	
	// Clean up
	_system::deleteGadgetHost();
	_system::deleteKeyboard();
	
	// Create new state
	switch( _systemController::state )
	{
		case _systemState::bootup:
			_systemController::scenario = new _scBootup();
			break;
		case _systemState::login:
			_systemController::scenario = new _scLogin();
			break;
		case _systemState::setup:
			_systemController::scenario = new _scSetup();
			break;
		case _systemState::desktop:
			_systemController::scenario = new _scDesktop();
			break;
		default:
			break;
	}
	
	if( _systemController::dirty )
		goto start;
	
	// Fade up Screen
	if( _systemController::state != _systemState::empty )
		_system::fadeMainScreen( true );
	
	irqEnable( IRQ_VBLANK ); // Leave critical Section
}

void _systemController::main()
{
	changeState( _systemState::desktop );
	static int i = 0;
	
	while( true )
	{
		_systemController::controllerVBL();
		
		// Run Programs
		_system::runPrograms();
		
		// Refresh System-Usage
		_system::_cpuUsageTemp_ = ( _system::_cpuUsageTemp_ + _system::_cpuUsageTemp_ + REG_VCOUNT - 192 ) / 3;
		
		// wait until line 0 
		swiIntrWait( 0, IRQ_VCOUNT );
		
		// Wait for VBlank
		swiWaitForVBlank();
		
		if( ++i > 200 
			&& false 
		)
		{
			const unsigned int FreeMemSeg=1024;
			
			unsigned int s;
			for( s = FreeMemSeg ; s < 4096 * 1024 ; s += FreeMemSeg )
			{
				void *ptr = malloc( FreeMemSeg );
				if( !ptr )
				{
					printf("\x1b[2J");
					printf("free: %d\n",s-FreeMemSeg);
					break;
				}
			}
			while(true);
		}
		
		if( ++i > 200 
			&& false 
		)
		{
			_codeAnalyzer::printResults();
			while(true);
		}
	}
}

_systemController::_systemState 	
				_systemController::state = _systemController::_systemState::empty; // I know, that one is quite long ;)
bool 			_systemController::dirty = false;
_scenario*		_systemController::scenario	= nullptr;