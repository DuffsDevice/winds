#ifndef _WIN_T_SYSTEM_
#define _WIN_T_SYSTEM_

#include "_type/type.h"
#include "_type/type.direntry.h"
#include "_type/type.animation.h"
#include "_type/type.progLua.h"
#include "_type/type.progC.h"
#include "_type/type.event.h"
#include "_type/type.runtimeAttributes.h"
#include "_type/type.registry.h"
#include "_type/type.gadgetScreen.h"
#include "_type/type.freetypefont.h"
#include "_gadget/gadget.keyboard.h"
#include "fat.h"

#include <tr1/memory>

#include <list>

class _system{
	
	private:
		
		friend class _freetypefont;
		static bool sleeping;
		
		//! Attributes
		static list< _animation >	_animations_;
		static list<pair<_program*
					,_cmdArgs>> 	_programs_;
		static _direntry*			_debugFile_;
		static _gadget*				_currentFocus_;
		
		//! Events
		static bool 				eventThrowable;
		static list<_gadgetEvent> 	events;
		static list<_gadgetEvent> 	newEvents;
		static void enableEventThrowing( void );
		static void processEvents();
		static void disableEventThrowing( void );
		
		//! Process User Inputs
		static void processInput();
		
		static void runAnimations();
		static void runPrograms();
		static void displayMemUsage();
		
		static void _vblank_();
		
		friend class _gadget;
		friend class _program;
		friend class _animation;
		
		//! Add Thinks for execution
		static void executeAnimation( const _animation& anim );
		static void executeProgram( _program* prog , _cmdArgs args = _cmdArgs() );
		static void generateEvent( _gadgetEvent event );
		
		//! Things for termination
		static void terminateProgram( _program* prog );
		static void terminateAnimation( const _animation& anim );
		
		static int 	bgIdFront;
		static int 	bgIdBack;
		static int 	bgIdSub;
		
	public:
	
		static _gadgetScreen*			_gadgetHost_;
		static _keyboard*				_keyboard_;
		static _screen*					_topScreen_;
		static _registry*				_registry_;
		static _runtimeAttributes*		_runtimeAttributes_;
		
		// Constructor
		_system();
		
		// Destructor
		~_system();
		
		//! Get a Built in Program
		static _program* getBuiltInProgram( string qualifiedName );
		
		//! Get Current Time (time since system startup)
		static _u32 getTime();
		
		//! Obtain current Keys
		static _u16 getCurrentKeys();
		
		//! Turn Device off
		static void shutDown();
		
		//! main Loop...
		static void main();
		
		//! Press Any Key to continue...
		static void submit();
		
		//! Debugging
		static void debug( string msg );
};

extern _system* _system_;

#endif