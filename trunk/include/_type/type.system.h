#ifndef _WIN_T_SYSTEM_
#define _WIN_T_SYSTEM_

#include "_type/type.h"
#include "_type/type.callback.h"
#include "_type/type.animation.h"
#include "_type/type.program.h"
#include "_type/type.gadgetScreen.h"
#include "_type/type.event.h"
#include "_type/type.font.h"
#include "_type/type.runtimeAttributes.h"

#include <memory>

class _direntry;
class _registry;
class _keyboard;
class _user;

typedef _vector<_pair<const _callback*,_callbackData>>	_timerList;
typedef _vector<pair<_program*,_programData>>			_programList;
typedef _vector<_animation*>							_animationList;

//! For inlining even functions with libnds
extern "C"{
extern _u32 cpuGetTiming();
extern _u32 keysHeld();
}

class _system{
	
	private:
	
		//! Attributes
		static _map<string,const _font*>	_fonts_;
		static _programList					_programs_;
		static _direntry*					_debugFile_;
		static _gadget*						_currentFocus_;
		static _gadget*						_lastClickedGadget_;
		
		//! Disables/enables the VBlank interrupt
		//! to not be interrupted during an important process
		static void enterCriticalSection();
		static void leaveCriticalSection();
		
		//! Internal
		static int 				_bgIdFront_;
		static int 				_bgIdBack_;
		static int 				_bgIdSub_;
		static int 				_cpuUsageTemp_;
		static string 			_curLanguageShortcut_;
		static bool 			_sleeping_;
		
		//! Timers
		static _timerList		_timers_;
		static _timerList		_timersToExecute_;
		
		//! Animations
		static _animationList	_animations_;
		static _animationList	_animationsToExecute_;
		
		//! Events
		static int				_curEventBuffer_;
		static _vector<_event> 	_eventBuffer_[2];
		
		static void eventsSwapBuffer(){ _curEventBuffer_ = !_curEventBuffer_; }
		static void optimizeEvents();
		static void processEvents();
		
		//! Screen-Animations
		static void fadeMainScreen( bool out , bool anim = true );
		
		//! Process User Inputs
		static void processInput();
		
		//! The daily Vertical Blank and its methods:
		static void runAnimations();
		static void runPrograms();
		static void runTimers();
		
		//! Unbind the old _gadgetHost_ from the DOM Tree and delete it
		static void deleteGadgetHost();
		static void deleteKeyboard();
		static void switchUser( _user* usr ) __attribute__(( nonnull(1) ));
		
		//! Misc...
		static void displayMemUsage();
		
		//! Internal Handler
		static void hblHandler();
		static void vblHandler();
		static void newHandler();
		
		//! Friends
		friend class _systemController;
		friend class _freetypefont;
		friend class _gadget;
		friend class _gadgetScreen;
		friend class _program;
		friend class _animation;
		friend class _progressbar;
		friend class _progLua;
		friend class _scLogin;
		friend class _scSetup;
		friend class _scDesktop;
		friend class _scBootup;
		friend class _keyboard;
		
		//! Add Thinks for execution
		static void executeAnimation( _animation* anim ) __attribute__(( nonnull(1) ));
		static void executeProgram( _program* prog , _cmdArgs&& args = _cmdArgs() ) __attribute__(( nonnull(1) ));
		static void generateEvent( _event&& event ) { _eventBuffer_[_curEventBuffer_].emplace_back( (_event&&)event ); }
		static void generateEvent( const _event& event ){ _system::generateEvent( _event( event ) ); }
		
	public:
	
		static _gadgetScreen*			_gadgetHost_;
		static _keyboard*				_keyboard_;
		static _screen*					_topScreen_;
		static _registry*				_registry_;
		static _registry*				_localizationTextTable_;
		static _registry*				_localizationMonthTable_;
		static _runtimeAttributes*		_rtA_; // _runtimeAttributes_
		
		static void removeEventsOf( _gadget* g ) __attribute__(( nonnull(1) ))
		{
			_vector<_event>& buff = _eventBuffer_[_curEventBuffer_];
			buff.erase(
				remove_if( buff.begin() , buff.end() , [&]( const _event& event )->bool{ return event.getDestination() == g; } ) /* Delete events */
				, buff.end()
			);
		}
		
		//! Constructor
		static void start();
		
		//! Destructor
		static void end();
		
		//! Execute the supplied command
		static bool executeCommand( string&& cmd );
		static bool executeCommand( const string& cmd ){ return _system::executeCommand( string( cmd ) ); }
		
		//! Get Current Time (milliseconds since system startup)
		static _tempTime getHighResTime();
		
		//! Get a Font by Name
		static const _font* getFont( string font )
		{
			if( font.empty() || !_fonts_.count( font ) )
				return _system::getFont();
			
			return _fonts_[font];
		}
		static const _font* getFont(){ return _fonts_[ _rtA_->getDefaultFontName() ]; }
		
		//! Get current Cpu-usage
		static _u8 getCpuUsage(){ return _cpuUsageTemp_; }
		
		//! Obtain current Keys
		static _u16 getCurrentKeys(){ return keysHeld() & (~(KEY_TOUCH|KEY_LID)); }
		
		//! Get string
		static string getLocalizedString( string name )
		{
			string value = _system::_localizationTextTable_->readIndex( name , _system::_curLanguageShortcut_ );
			if( value.empty() )
				return DSWindows::emptyStringSignature;
			return value;
		}
		
		//! Get localized month
		//! Pass month from 0 - 11
		static string getLocalizedMonth( _u8 month )
		{
			string value = _system::_localizationMonthTable_->readIndex( int2string( month ) , _system::_curLanguageShortcut_ );
			if( value.empty() )
				return DSWindows::emptyStringSignature;
			return value;
		}
		
		//! Execute the passed callback after duration d and repeat if requested
		template<typename T>
		static void executeTimer( T&& cb , _tempTime duration , bool isRepeating = false )
		{
			typedef typename T::_callback def; // Check if subclass of _callback
			_timersToExecute_.push_back(
				make_pair(
					new T( move( cb ) ), 
					_callbackData( { getHighResTime() , duration , isRepeating , false } )
				)
			);
		}
		
		//! Things for termination
		static void terminateProgram( _program* prog ) __attribute__(( nonnull(1) ));
		static void terminateTimer( const _callback& cb , bool luaStateRemove = false );
		
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
		
		//! Checks if the binary is executed on real hardware or on an emulator
		static bool isRunningOnEmulator();
		
		//! Debugging
		static void debug( const char* fmt , ... ) __attribute__(( format(gnu_printf, 1 , 2) ));
		
		//! Get the Currently Logged in _user object
		static const _user* getUser(){ return _system::_rtA_->getUser(); }
};

#endif