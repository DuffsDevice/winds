#ifndef _WIN_T_ANIMATION_
#define _WIN_T_ANIMATION_

#include <time.h>
#include "_type/type.h"
#include <functional>
#include <cmath>
#include <deque>

using namespace std;

// t = elapsed Time since Start
// b = startValue
// c = deltaValue
// d duration of the whole effect
class _animation{
	
	private:
	
		friend class _system;
		clock_t		startTime;
		_u32		duration; //! In Milliseconds
		_u32*		destination;
		
		//! Additionally: call a setter function
		function<void(int)> setterFunc;
		
		int			(*easeFunc)( float t , float b , float c , float d );
		
		int			fromValue;
		int			toValue;
		int			deltaValue;
		
		bool		runs;
		
		_u32		id;
		static _u32	idCount;
		
	public:
	
		_animation( int from , int to , _u32 dur );
		
		_u32 getID() const { return this->id; }
		
		//! Set Address of the Variable to write
		void setter( _u32* destination );
		
		void setter( function<void(int)> setterFunc ){ this->setterFunc = setterFunc; }
		
		//! Start the animation
		void start();
		
		//! Set fromValue
		void setFromValue( int fromValue ){
			this->fromValue = fromValue;
		}
		
		//! setToValue
		void setToValue( int toValue ){
			this->toValue = toValue;
		}
		
		//! Apply the Value
		void step( _u32 curTime )
		{
			if( !this->runs )
				return;
				
			_u32 tElapsed = curTime - this->startTime;
			
			if( tElapsed > this->duration )
			{
				this->runs = false;
				if( this->setterFunc != nullptr )
					this->setterFunc( this->toValue );
				if( this->destination != nullptr )
					*this->destination = this->toValue;
				return;
			}
			if( this->easeFunc != nullptr )
			{
				int value = this->easeFunc( tElapsed , this->fromValue , this->deltaValue , this->duration );
				if( this->setterFunc != nullptr )
					this->setterFunc(value);
				if( this->destination != nullptr )
					*this->destination = value;
			}
		}
		
		void setEasing( int (*easeFunc)( float t , float b , float c , float d ) ){
			this->easeFunc = easeFunc;
		}
		
		//! Check if animation is finished
		bool finished( _u32 timing ){ return timing - this->startTime > this->duration; }
		
		class _linear{
			public:
			static int ease( float t , float b , float c , float d );
		};
		
		class _quad{
			public:
			static int easeIn( float t , float b , float c , float d );
			static int easeOut( float t , float b , float c , float d );
			static int easeInOut( float t , float b , float c , float d );
		};
		
		class _sinus{
			public:
			static int easeIn( float t , float b , float c , float d );
			static int easeOut( float t , float b , float c , float d );
			static int easeInOut( float t , float b , float c , float d );
		};
		
		class _bounce{
			public:
			static int easeIn( float t , float b , float c , float d );
			static int easeOut( float t , float b , float c , float d );
			static int easeInOut( float t , float b , float c , float d );
		};
		
		class _back{
			public:
			static int easeIn( float t , float b , float c , float d );
			static int easeOut( float t , float b , float c , float d );
			static int easeInOut( float t , float b , float c , float d );
		};
		
		class _circular{
			public:
			static int easeIn( float t , float b , float c , float d );
			static int easeOut( float t , float b , float c , float d );
			static int easeInOut( float t , float b , float c , float d );
		};
		
		class _cubic{
			public:
			static int easeIn( float t , float b , float c , float d );
			static int easeOut( float t , float b , float c , float d );
			static int easeInOut( float t , float b , float c , float d );
		};
		
		class _elastic{
			public:
			static int easeIn( float t , float b , float c , float d );
			static int easeOut( float t , float b , float c , float d );
			static int easeInOut( float t , float b , float c , float d );
		};
		class _expo{
			public:
			static int easeIn( float t , float b , float c , float d );
			static int easeOut( float t , float b , float c , float d );
			static int easeInOut( float t , float b , float c , float d );
		};
};

#endif