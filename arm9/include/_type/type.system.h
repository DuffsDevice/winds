#ifndef _WIN_T_SYSTEM_
#define _WIN_T_SYSTEM_

#include "_type/type.h"
#include "_type/type.callback.h"
#include "_type/type.program.h"
#include "_type/type.gadget.screen.h"
#include "_type/type.event.h"
#include "_type/type.font.h"
#include "_type/type.runtimeattributes.h"
#include "_type/type.key.h"
#include "_type/type.language.h"
#include "_type/type.registry.h"

class _ini;
class _iniFile;
class _direntry;
class _keyboard;
class _user;

typedef _vector<_pair<_event,_eventCallType>> _eventList;

//! Predefines of libnds
extern "C"{
	extern _u32 cpuGetTiming();
	extern _u32 keysHeld();
}

//! Enumerates the underlying hardware
enum class _hardwareType{
	emulator,
	ds,
	dsi
};
extern _toStr<_hardwareType> hardwareType2str;

class _system{
	
	private:
	
		//! Attributes
		static _map<string,_fontPtr>		_fonts_;
		static _direntry*					_debugFile_;
		static _gadget*						_currentFocus_;
		static _gadget*						_lastClickedGadget_;
		static _screen*						_topScreen_;
		static _registry*					_registry_;
		static _runtimeAttributes*			_rtA_; // _runtimeAttributes_
		
		//! Displayed as replacement if a language-specific term
		//! is not available in the currently selected language
		static const string					_emptyStringSignature_;
		
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
		
		//! Events
		static int				_curEventBuffer_;
		static _eventList	 	_eventBuffer_[2];
		
		static void eventsSwapBuffer(){ _curEventBuffer_ = !_curEventBuffer_; }
		static void optimizeEvents();
		static void processEvents();
		
		//! Screen-Animations
		static void fadeMainScreen( bool out , bool anim = true );
		
		//! Process User Inputs
		static void processInput();
		
		//! Unbind the old _gadgetHost_ from the DOM Tree and delete it
		static void deleteGadgetHost();
		static void deleteKeyboard();
		static void switchUser( _user&& usr );
		
		//! Misc...
		static void displayMemUsage();
		
		//! Internal Handler
		static void vblHandler();
		static void newHandler();
		
		//! Friends
		friend class _systemController;
		friend class _freetypeFont;
		friend class _gadget;
		friend class _gadgetScreen;
		friend class _program;
		friend class _animation;
		friend class _progressBar;
		friend class _progLua;
		friend class _scLogin;
		friend class _scSetup;
		friend class _scDesktop;
		friend class _scBootup;
		
		//! Add Thinks for execution
		static void generateEvent( _event event , _eventCallType callType ){
			_eventBuffer_[_curEventBuffer_].emplace_back( make_pair(move(event) , callType) );
		}
		
		// Makes sure that no events concerning a gadget are on the buffer anymore (for _gadget::~_gadget)
		static void removeEventsOf( _gadget* g ) __attribute__(( nonnull(1) ))
		{
			_eventList& buff = _eventBuffer_[_curEventBuffer_];
			buff.erase(
				remove_if( buff.begin() , buff.end()
					, [&]( const _pair<_event,_eventCallType>& event )->bool{ return event.first.getDestination() == g; } /* Delete events */
				)
				, buff.end()
			);
		}
		
	public:
		
		//! More attributes
		static _gadgetScreen*		_gadgetHost_;
		static _keyboard*			_keyboard_;
		static _ini*				_localizationTextTable_;
		static _ini*				_localizationMonthTable_;
		
		//! Constructor
		static void					start();
		
		//! Destructor
		static void					end();
		
		//! Execute the supplied command
		static bool					executeCommand( const string& cmd );
		
		//! Get time since system startup
		static _tempTime			getMilliTime();	//! in Milliseconds
		static _tempTime			getMicroTime();	//! in Microseconds since system startup
		static _tempTime			getRawTime();	//! Clock Ticks at 33.513982 GHz
		
		//! Get a Font by Name
		static _fontPtr			getFont( const string& font )
		{
			if( font.empty() || !_fonts_.count( font ) )
				return _system::getFont();
			
			return _fonts_[font];
		}
		static _fontPtr			getFont(){ return _fonts_[ _rtA_->getDefaultFontName() ]; }
		
		//! Get current Cpu-usage
		static _u8					getCpuUsage(){ return _cpuUsageTemp_; }
		
		//! Obtain current Keys
		static _hardwareKeyPattern	getCurrentKeys(){ return _hardwareKeyPattern( keysHeld() & (~(KEY_TOUCH|KEY_LID)) ); }
		
		//! Get the currently focused gadget
		static _gadget*				getCurrentFocus(){ return _system::_currentFocus_; }
		
		//! Get string
		static const string&		getLocalizedString( const string& name )
		{
			const string& value = _system::_localizationTextTable_->readIndex( name , _system::_curLanguageShortcut_ );
			if( value.empty() )
				return _system::_emptyStringSignature_;
			return value;
		}
		
		//! Get localized month
		//! Pass month from 0 - 11
		static const string&		getLocalizedMonth( _u8 month )
		{
			const string& value = _system::_localizationMonthTable_->readIndex( int2string( month ) , _system::_curLanguageShortcut_ );
			if( value.empty() )
				return _system::_emptyStringSignature_;
			return value;
		}
		
		//! Turn Device off
		static void					shutDown();
		
		//! main Loop...
		static void					main();
		
		//! Press Any Key to continue...
		static void					submit();
		
		//! Set Language
		static void					setLanguage( _language );
		
		//! Get the current Langauge
		static _language			getLanguage();
		
		//! Get the name of the user within the DS internal firmware
		static const string&		getDSUserName();
		
		//! Check, on which hardware type the executeable is running on
		static _hardwareType		getHardwareType();
		
		//! Debugging
		static void					debug( const char* fmt , ... ) __attribute__(( format(gnu_printf, 1 , 2) ));
		static void					vdebug( const char* fmt , va_list );
		
		//! Get the Currently Logged in _user object
		static const _user&			getUser(){ return _system::_rtA_->getUser(); }
		
		//! And some getters...
		static _screen*				getTopScreen(){ return _system::_topScreen_; }
		static _registry& 			getRegistry(){ return *_system::_registry_; }
		static _runtimeAttributes&	getRTA(){ return *_system::_rtA_; }
};

#endif