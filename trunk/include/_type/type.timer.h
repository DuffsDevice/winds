#ifndef _WIN_T_TIMER_
#define _WIN_T_TIMER_

#include "_type/type.h"
#include "_type/type.callback.h"

// List of pointers to timers
typedef _vector<class _timer*> _timerList;

class _timer
{
	private:
		
		_tempTime	startTime;
		_tempTime	duration;
		bool 		repeating;
		bool		runs;
		
		_callback<void()>* callback;
		
		static _timerList globalTimers;
		static _timerList globalTimersToExecute;
		static void runTimers();
		
		friend class _systemController;
		friend class _system;
	
	public:
		
		//! Ctor
		template<typename T>
		_timer( T&& cb , _tempTime duration , bool repeating ) :
			duration( duration )
			, repeating( repeating )
			, runs( false )
		{
			typedef typename std::remove_reference<T>::type T2; // Convert to pure type
			typedef typename T2::_callback def; // Check if subclass of _callback
			
			this->callback = new T( move(cb) );
		}
		
		//! Default Ctor
		_timer( _tempTime duration = 0 , bool repeating = false ) : 
			duration( duration )
			, repeating( repeating )
			, runs( false )
			, callback( nullptr )
		{}
		
		//! Move constructor
		_timer( _timer&& tmr ) :
			callback( nullptr )
		{
			*this = (_timer&&)tmr;
		}
		
		//! Delete copy constructor
		_timer( const _timer& ) = delete;
		
		//! Assignment operator
		_timer& operator=( _timer&& );
		
		//! Delete copy assignment operator
		_timer& operator=( _timer& ) = delete;
		
		//! Dtor
		~_timer(){
			this->stop();
			this->deleteCallback();
		}
		
		//! Terminates all instances of this timer that are currently running
		void stop();
		
		//! Starts the timer
		void start();
		
		//! Sets the callback to call if the timer fires
		template<typename T>
		void setCallback( T&& cb )
		{
			typedef typename std::remove_reference<T>::type T2; // Convert to pure type
			typedef typename T2::_callback def; // Check if subclass of _callback
			
			this->deleteCallback(); // Delete old one
			this->callback = new T( move(cb) ); // Create new one
		}
		
		//! Delete the contained callback
		void deleteCallback()
		{
			if( this->callback )
				delete this->callback;
			this->callback = nullptr;
		}
		
		//! Set duration after which the callback should be called
		void setDuration( _tempTime duration ){
			this->duration = duration;
		}
		
		//! Get duration after which the callback is called
		_tempTime getDuration(){ return this->duration; }
		
		//! Set whether the callback should repeatingly be called
		void setRepeating( bool repeating = true ){
			this->repeating = repeating;
		}
		
		//! Check if the timer is set on repeat
		bool isRepeating(){ return this->repeating; }
		
		//! Check whether the timer is currently running
		bool isRunning(){ return this->runs; }
};



#endif