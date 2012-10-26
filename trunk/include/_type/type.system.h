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

class _system{
	
	private:
		
		friend class _freetypefont;
		static bool sleeping;
		
		//! Attributes
		static _list<_animation*>	_animations_;
		static _map<string,_font*>	_fonts_;
		static _list<pair<_program*
					,_cmdArgs>> 	_programs_;
		static _direntry*			_debugFile_;
		static _gadget*				_currentFocus_;
		
		//! Events
		static int					curEventBuffer;
		static _list<_gadgetEvent> 	eventBuffer[2];
		static void eventsSwapBuffer( void );
		static void processEvents();
		
		//! Process User Inputs
		static void processInput();
		
		static void runAnimations();
		static void runPrograms();
		static void displayMemUsage();
		
		//! Different Scenarios
		static void initializeComponents();
		static void setup();
		static void loginPage();
		
		//! Unbind the old _gadgetHost_ from the DOM Tree and delete it
		static void deleteGadgetHost();
		
		static void _vblank_();
		static void _newErrorFunc_();
		
		friend class _gadget;
		friend class _program;
		friend class _animation;
		
		//! Add Thinks for execution
		static void executeAnimation(_animation* anim );
		static void executeProgram( _program* prog , _cmdArgs args = _cmdArgs() );
		static void generateEvent( _gadgetEvent event );
		
		//! Things for termination
		static void terminateProgram( _program* prog );
		static void terminateAnimation( const _animation* anim );
		
		static int 	bgIdFront;
		static int 	bgIdBack;
		static int 	bgIdSub;
		
		static int 	cpuUsageTemp;
		
	public:
	
		static _gadgetScreen*			_gadgetHost_;
		static _keyboard*				_keyboard_;
		static _screen*					_topScreen_;
		static _registry*				_registry_;
		static _runtimeAttributes*		_runtimeAttributes_;
		
		static void removeEventsOf( _gadget* g );
		
		// Constructor
		_system();
		
		// Destructor
		~_system();
		
		//! Get a Built in Program
		static _program* getBuiltInProgram( string qualifiedName );
		
		//! Get Current Time (time since system startup)
		static _u32 getTime();
		
		//! Get a Font by Name
		static _font* getFont( string font );
		static _font* getFont();
		
		static _u32 getCpuUsage();
		
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