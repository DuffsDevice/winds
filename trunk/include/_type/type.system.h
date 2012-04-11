#ifndef _WIN_T_SYSTEM_
#define _WIN_T_SYSTEM_

#include "_type/type.h"
#include "_type/type.file.h"
#include "_type/type.animation.h"
#include "_type/type.progLua.h"
#include "_type/type.progC.h"
#include "_type/type.event.h"
#include "_type/type.runtimeAttributes.h"
#include "_gadget/gadget.windows.h"
#include "fat.h"

#include <deque>

class _system{
	
	private:
		
		static bool sleeping;
		
		static _animationsGroup<int> 	_animations_;
		static deque<_program*> 		_programs_;
		static map<string,_program*>		_assocPrograms_;
		static _windows*				_windows_;
		static _file*					_debugFile_;
		
		//! Process User Inputs
		static void processInput();
		
		//! Events
		static deque<_gadgetEvent> 		events;
		static deque<_gadgetEvent> 		newEvents;
		static bool eventThrowable;
		
		static void optimizeEvents();
		static void enableEventThrowing( void );
		static void processEvents();
		static void disableEventThrowing( void );
		
		// Benchmarks
		static void benchMarkStart();
		static void benchMarkStopPrint();
		
		static void runPrograms();
		static void displayMemUsage();
		
	public:
	
		static _runtimeAttributes*		_runtimeAttributes_;
		
		// Constructor
		_system();
		
		static void addAnimation( _animation<int>* anim );
		
		static void addProgram( _program* prog );
		
		static void addEvent( _gadgetEvent event );
		
		static _u32 getNow();
		
		// Obtain current Keys
		static _u16 getCurrentKeys();
		
		static void setWindows( _windows* win );
		
		static void _vblank_();
		
		static void goToSleep();
		
		static void wakeUp();
		
		static void shutDown();
		
		static void restart();
		
		static bool initFat();
		
		static void initWifi();
		
		static void initDisplay();
		
		static void setBacklight( _u8 level );
		
		static void run();
		
		static bool runProgram( string qualifiedName , _cmdArgs args = _cmdArgs() );
		
		//! Press Any Key to continue...
		static void submit();
		
		static void debug( string msg );
};

extern _system* _system_;

#endif