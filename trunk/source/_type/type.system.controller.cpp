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
	changeState( _systemState::setup );
	static int i = 0;
	static int s = 0;
	
	while( true )
	{		
		//! Change State
		_systemController::checkDirty();
		
		// Let the Processor do its work!
		_timer::runTimers();
		_system::processEvents();
		_system::runPrograms();
		
		// Compute System-Usage
		//_system::_cpuUsageTemp_ = ( _system::_cpuUsageTemp_ + _system::_cpuUsageTemp_ + REG_VCOUNT - 192 ) / 3;
		
		// wait until line 0 
		swiIntrWait( 0, IRQ_VBLANK );
		//swiIntrWait( 0, IRQ_VCOUNT );
		//swiIntrWait( 0, IRQ_HBLANK );
		
		// Wait for VBlank
		//swiWaitForVBlank();
		//swiWaitForIRQ();
		
		i++;
		//if( i == 60 )
		//{
		//	s++;
		//	i = 0;
		//	printf("main: %d\n",s);
		//}
		
		//printf("main");
		
		//printf("CF: %s\n",_system::_currentFocus_?gadgetType2string[_system::_currentFocus_->getType()].c_str():"nothing");
		
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
		if( i > 200 
			&& false 
		)
		{
			_codeAnalyzer::printResults();
			while(true);
		}
		#endif
	}
}

_systemController::_systemState 	
				_systemController::state = _systemController::_systemState::empty; // I know, that one is quite long ;)
bool 			_systemController::dirty = false;
_scenario*		_systemController::scenario	= nullptr;