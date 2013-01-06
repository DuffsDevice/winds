#ifndef _WIN_T_SYSTEM_
#define _WIN_T_SYSTEM_

#include "_type/type.h"
#include "_type/type.direntry.h"
#include "_type/type.callback.h"
#include "_type/type.animation.h"
#include "_type/type.program.h"
#include "_type/type.event.h"
#include "_type/type.runtimeAttributes.h"
#include "_type/type.registry.h"
#include "_type/type.gadgetScreen.h"
#include "_type/type.freetypefont.h"
#include "_type/type.system.controller.h"
#include "_gadget/gadget.keyboard.h"

//! For inlining even functions with libnds
extern "C"{
extern _u32 cpuGetTiming();
extern _u32 keysHeld();
}

class _system{
	
	private:
	
		//! Attributes
		static _list<_animation*>				_animations_;
		static _list<_pair<const _callback*,
						_callbackData>>			_timers_;
		static _map<string,_font*>				_fonts_;
		static _list<_pair<_program*,_cmdArgs>>	_programs_;
		static _direntry*						_debugFile_;
		static _gadget*							_currentFocus_;
		
		//! Internal
		static int 								_bgIdFront_;
		static int 								_bgIdBack_;
		static int 								_bgIdSub_;
		static int 								_cpuUsageTemp_;
		static string 							_curLanguageShortcut_;
		static bool 							_sleeping_;
		
		//! Events
		static int								_curEventBuffer_;
		static _list<_event> 					_eventBuffer_[2];
		
		static void eventsSwapBuffer(){ _curEventBuffer_ = !_curEventBuffer_; }
		static void processEvents();
		
		//! Screen-Animations
		static void fadeMainScreen( bool out , bool anim = true );
		
		//! Process User Inputs
		static void processInput();
		
		//! The daily Vertical Blank and its methods:
		static void runAnimations(){ _animations_.remove_if( [&]( _animation* anim )->bool{ anim->step( getHighResTime() ); return anim->finished(); } ); }
		static void runTimers();
		static void runPrograms();
		
		//! Unbind the old _gadgetHost_ from the DOM Tree and delete it
		static void deleteGadgetHost();
		static void deleteKeyboard();
		
		//! Misc...
		static void displayMemUsage();
		
		//! Internal Handler
		static void vblHandler();
		static void newHandler();
		
		//! Friends
		friend class _systemController;
		friend class _freetypefont;
		friend class _gadget;
		friend class _program;
		friend class _animation;
		friend class _progressbar;
		friend class _progLua;
		
		//! Add Thinks for execution
		static void executeAnimation(_animation* anim ){
			_animations_.remove( anim );
			_animations_.push_back( anim );
		}
		static void executeProgram( _program* prog , _cmdArgs args = _cmdArgs() )
		{
			_programs_.push_back( make_pair( prog , args ) );
			prog->main( _gadgetHost_ , args );
		}
		static void generateEvent( _event event ){ _eventBuffer_[_curEventBuffer_].push_back( event ); }
		
		//! Things for termination
		static void terminateProgram( _program* prog ){ 
			// Erase the Program-Instance in the list of running instances
			_programs_.remove_if( [&]( pair<_program*,_cmdArgs> s )->bool{ return s.first == prog; } );
			delete prog;
		}
		static void terminateAnimation( _animation* anim ){ _animations_.remove( anim ); }
		static void terminateTimer( const _callback& cb );
		
	public:
	
		static _gadgetScreen*			_gadgetHost_;
		static _keyboard*				_keyboard_;
		static _screen*					_topScreen_;
		static _registry*				_registry_;
		static _ini*					_localizationTable_;
		static _runtimeAttributes*		_runtimeAttributes_;
		
		static void removeEventsOf( _gadget* g )
		{
			_eventBuffer_[_curEventBuffer_].remove_if( [&]( _event event )->bool{ return event.getDestination() == g; } ); // Delete events
		}
		
		//! Constructor
		static void start();
		
		//! Destructor
		static void end();
		
		//! Get a Built in Program
		static _program* getBuiltInProgram( string qualifiedName );
		
		//! Get Current Time (milliseconds since system startup)
		static _tempTime getHighResTime();
		
		//! Get a Font by Name
		static _font* getFont( string font )
		{
			if( font.empty() || !_fonts_.count( font ) )
				return _fonts_[ _runtimeAttributes_->defaultFont ];
			
			return _fonts_[font];
		}
		static _font* getFont(){ return _fonts_[ _runtimeAttributes_->defaultFont ]; }
		
		static _u8 getCpuUsage(){ return _cpuUsageTemp_; }
		
		//! Obtain current Keys
		static _u16 getCurrentKeys(){ return keysHeld() & (~(KEY_TOUCH|KEY_LID)); }
		
		//! Get string
		static const string& getLocalizedString( string name );
		
		//! Execute the passed callback after duration d and repeat if requested
		static void executeTimer( const _callback* cb , _tempTime d , bool isRepeating = false );
		
		//! Turn Device off
		static void shutDown();
		
		//! main Loop...
		static void main();
		
		//! Press Any Key to continue...
		static void submit();
		
		//! Set Language
		static void setLanguage( _language );
		
		//! Get the current Langauge
		static _language getLanguage();
		
		//! Debugging
		static void debug( string msg );
};

#endif