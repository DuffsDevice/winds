#ifndef _WIN_T_TIMER_
#define _WIN_T_TIMER_

#include <_type/type.h>
#include <_type/type.callback.h>
#include <_type/type.uniqueptr.h>
#include <_type/type.paramalloc.h>

class _timer
{
	private:
		
		_tempTime						startTime;
		_tempTime						duration;
		bool 							repeating;
		bool							runs;
		_uniquePtr<_callback<void()>>	callback;
		
		friend class _timerController;
	
	public:
		
		//! Ctor
		_timer( _paramAlloc<_callback<void()>> cb , _tempTime duration , bool repeating ) :
			duration( duration )
			, repeating( repeating )
			, runs( false )
			, callback( cb.get() )
		{}
		
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
		}
		
		//! Terminates all instances of this timer that are currently running
		void stop();
		
		//! Starts the timer
		void start();
		
		//! Sets the callback to call if the timer fires
		void setCallback( _paramAlloc<_callback<void()>> cb ){
			this->callback = cb.get(); // Create new one
		}
		
		//! Delete the contained callback
		void deleteCallback(){
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