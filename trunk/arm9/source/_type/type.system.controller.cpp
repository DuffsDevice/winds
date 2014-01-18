#include "_type/type.system.h"
#include "_type/type.system.controller.h"
#include "_type/type.timer.h"
#include "_gadget/gadget.keyboard.h"

// NDS Interrupts
#include <nds/interrupts.h>
#include <nds/bios.h>

void _systemController::checkDirty()
{
	if( !dirty )
		return;
	
	_system::enterCriticalSection();
	
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
	
	_system::leaveCriticalSection();
}

void _systemController::main()
{
	changeState( _systemState::desktop );
	
	static int i = 0;
	while( true )
	{
		i++;
		
		// If needed Change State
		_systemController::checkDirty();
		
		// Wait until the Screen Cursor reaches Line 0
		swiIntrWait( 0 , IRQ_VCOUNT );
		
		// Let the Processor do its work!
		_timer::runTimers();
		_system::processEvents();
		_program::runPrograms();
		
		// Compute System-Usage
		_system::_cpuUsageTemp_ = _system::_cpuUsageTemp_ * 15 + 100 - (REG_VCOUNT * 100 / 256 );
		_system::_cpuUsageTemp_ >>= 4;
		
		// Wait for VBlank
		swiWaitForVBlank();
		
		if( i > 200 
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
		#ifdef DEBUG_PROFILING
		if( i > 200 )
		{
			_codeAnalyzer::printResults();
			while(true);
		}
		#endif
	}
}

_systemState 	_systemController::state = _systemState::empty; // I know, that one is quite long ;)
bool 			_systemController::dirty = false;
_scenario*		_systemController::scenario	= nullptr;