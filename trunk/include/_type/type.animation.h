#ifndef _WIN_T_ANIMATION_
#define _WIN_T_ANIMATION_

#include <time.h>
#include "_type/type.h"
#include <functional>
//#include <deque>

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
		_s32*		destination;
		
		//! Additionally: call a setter function
		function<void(int)> setterFunc;
		function<void(int)> finishFunc;
		
		_float		(*easeFunc)( _float t , _float b , _float c , _float d );
		
		int			fromValue;
		int			toValue;
		int			deltaValue;
		
		bool		runs;
		
	public:
	
		_animation( _s32 from , _s32 to , _u32 dur );
		
		~_animation();
		
		//! Set Address of the Variable to write
		void setter( _s32* destination );
		
		//! Set a lamda-expression to be the setter
		void setter( function<void(int)> setterFunc ){ this->setterFunc = setterFunc; }
		
		//! Set a lamda-expression to be called at the end of the animation
		void finish( function<void(int)> finishFunc = NULL ){ this->finishFunc = finishFunc; }
		
		//! Start the animation
		void start();
		
		//! Terminate the animation (cannot be restarted from the point of termination)
		void terminate();
		
		//! Set fromValue
		void setFromValue( int fromValue ){
			this->fromValue = fromValue;
		}
		
		//! setToValue
		void setToValue( int toValue ){
			this->toValue = toValue;
		}
		
		//! Apply the Value
		void step( _u32 curTime );
		
		void setEasing( _float (*easeFunc)( _float t , _float b , _float c , _float d ) ){
			this->easeFunc = easeFunc;
		}
		
		//! Check if animation is finished
		bool finished(){ return !this->runs; }
		
		class _linear{
			public:
			static _float ease( _float t , _float b , _float c , _float d );
		};
		
		class _quad{
			public:
			static _float easeIn( _float t , _float b , _float c , _float d );
			static _float easeOut( _float t , _float b , _float c , _float d );
			static _float easeInOut( _float t , _float b , _float c , _float d );
		};
		
		class _sinus{
			public:
			static _float easeIn( _float t , _float b , _float c , _float d );
			static _float easeOut( _float t , _float b , _float c , _float d );
			static _float easeInOut( _float t , _float b , _float c , _float d );
		};
		
		class _bounce{
			public:
			static _float easeIn( _float t , _float b , _float c , _float d );
			static _float easeOut( _float t , _float b , _float c , _float d );
			static _float easeInOut( _float t , _float b , _float c , _float d );
		};
		
		class _back{
			public:
			static _float easeIn( _float t , _float b , _float c , _float d );
			static _float easeOut( _float t , _float b , _float c , _float d );
			static _float easeInOut( _float t , _float b , _float c , _float d );
		};
		
		class _circular{
			public:
			static _float easeIn( _float t , _float b , _float c , _float d );
			static _float easeOut( _float t , _float b , _float c , _float d );
			static _float easeInOut( _float t , _float b , _float c , _float d );
		};
		
		class _cubic{
			public:
			static _float easeIn( _float t , _float b , _float c , _float d );
			static _float easeOut( _float t , _float b , _float c , _float d );
			static _float easeInOut( _float t , _float b , _float c , _float d );
		};
		
		class _elastic{
			public:
			static _float easeIn( _float t , _float b , _float c , _float d );
			static _float easeOut( _float t , _float b , _float c , _float d );
			static _float easeInOut( _float t , _float b , _float c , _float d );
		};
		
		class _expo{
			public:
			static _float easeIn( _float t , _float b , _float c , _float d );
			static _float easeOut( _float t , _float b , _float c , _float d );
			static _float easeInOut( _float t , _float b , _float c , _float d );
		};
};

#endif