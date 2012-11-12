#ifndef _WIN_T_SYSTEM_
#define _WIN_T_SYSTEM_

#include "_type/type.h"
#include "_type/type.direntry.h"
#include "_type/type.callback.h"
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
		static bool 				sleeping;
		static string 				curLanguageShortcut;
		
		//! Attributes
		static _list<_animation*>	_animations_;
		static _list<const 
					_callback*>		_vblListeners_;
		static _map<string,_font*>	_fonts_;
		static _list<pair<_program*
					,_cmdArgs>> 	_programs_;
		static _direntry*			_debugFile_;
		static _gadget*				_currentFocus_;
		
		//! Events
		static int					curEventBuffer;
		static _list<_event> 	eventBuffer[2];
		static void eventsSwapBuffer(){ _system::curEventBuffer = !_system::curEventBuffer; }
		static void processEvents();
		
		//! Screen-Animations
		static void fadeMainScreen( bool out , bool anim = true );
		
		//! Process User Inputs
		static void processInput();
		
		static void runAnimations(){ _system::_animations_.remove_if( [&]( _animation* anim )->bool{ anim->step( _system::getTime() ); return anim->finished(); } ); }
		static void runPrograms(){
			_system::_programs_.remove_if( 
				[]( pair<_program*,_cmdArgs>& prog )->bool{
					if( prog.first->main( prog.second ) == 1 && prog.first->autoDelete ){ 
						delete prog.first; return true; 
					}
					return false;
				}
			);
		}
		static void runVblListeners(){
			for( const _callback* cb : _system::_vblListeners_ )
				(*cb)();
		}
		static void displayMemUsage();
		
		//! Different Scenarios
		static void initializeComponents();
		static void setup();
		static void loginPage();
		
		//! Unbind the old _gadgetHost_ from the DOM Tree and delete it
		static void deleteGadgetHost();
		static void deleteKeyboard();
		
		
		static void _vblank_();
		static void _newErrorFunc_();
		
		friend class _gadget;
		friend class _program;
		friend class _animation;
		friend class _progressbar;
		friend class _progLua;
		
		//! Add Thinks for execution
		static void executeAnimation(_animation* anim ){
			_system::_animations_.remove_if( [&]( _animation* a )->bool{ return a == anim; } );
			_system::_animations_.push_back( anim );
		}
		static void executeProgram( _program* prog , _cmdArgs args = _cmdArgs() )
		{
			_system::_programs_.push_back( make_pair( prog , args ) );
			prog->init( _system::_gadgetHost_ , args );
		}
		static void addVblListener( const _callback* cb ){
			_system::_vblListeners_.push_back( cb );
		}
		static void generateEvent( _event event ){ _system::eventBuffer[_system::curEventBuffer].push_back( event ); }
		
		//! Things for termination
		static void terminateProgram( _program* prog ){ 
			// Erase the Program-Instance in the list of running instances
			_system::_programs_.remove_if( [&]( pair<_program*,_cmdArgs> s )->bool{ return s.first == prog; } );
			delete prog;
		}
		static void removeVblListener( const _callback& cb )
		{
			_system::_vblListeners_.remove_if( [&]( const _callback* data )->bool{ if( *data == cb ){ delete data; return true; } return false; } );
		}
		static void terminateAnimation( const _animation* anim ){ _system::_animations_.remove_if( [&]( _animation* a )->bool{ return a == anim; } ); }
		
		static int 	bgIdFront;
		static int 	bgIdBack;
		static int 	bgIdSub;
		
		static int 	cpuUsageTemp;
		
	public:
	
		static _gadgetScreen*			_gadgetHost_;
		static _keyboard*				_keyboard_;
		static _screen*					_topScreen_;
		static _registry*				_registry_;
		static _ini*					_localizationTable_;
		static _runtimeAttributes*		_runtimeAttributes_;
		
		static void removeEventsOf( _gadget* g )
		{
			// Delete events
			_system::eventBuffer[_system::curEventBuffer].remove_if( [&]( _event event )->bool{ return event.getDestination() == g; } );
		}
		
		// Constructor
		_system();
		
		// Destructor
		~_system();
		
		//! Get a Built in Program
		static _program* getBuiltInProgram( string qualifiedName );
		
		//! Get Current Time (time since system startup)
		static _time getTime();
		
		//! Get a Font by Name
		static _font* getFont( string font )
		{
			if( font.empty() || !_system::_fonts_.count( font ) )
				return _system::_fonts_[ _system::_runtimeAttributes_->defaultFont ];
			
			return _system::_fonts_[font];
		}
		static _font* getFont(){ return _system::_fonts_[ _system::_runtimeAttributes_->defaultFont ]; }
		
		static _u8 getCpuUsage(){ return _system::cpuUsageTemp; }
		
		//! Obtain current Keys
		static _u16 getCurrentKeys();
		
		//! Get string
		static string& getLocalizedString( string name );
		
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

extern _system* _system_;

#endif