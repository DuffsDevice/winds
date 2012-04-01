#ifndef _WIN_T_ANIMATION_
#define _WIN_T_ANIMATION_

#include <time.h>
#include "_type/type.h"
#include <functional>
#include <cmath>
#include <deque>

using namespace std;

template<typename T> class _animationsGroup;

// t = elapsed Time since Start
// b = startValue
// c = deltaValue
// d duration of the whole effect
template<typename T>
class _animation{
	
	private:
	
		friend class _animationsGroup<T>;
		clock_t	startTime;
		_u32	duration; //! In Milliseconds
		_u32*	destination;
		
		//! Additionally: call a setter function
		function<void(T)> setterFunc;
		
		T 		(*easeFunc)( float t , float b , float c , float d );
		
		T		fromValue;
		T		toValue;
		T		deltaValue;
		
		bool	runs;
		
	public:
	
		_animation( T from , T to , _u32 dur ) :
			startTime( 0 ) , duration( dur ) , destination( nullptr ) , setterFunc( nullptr ) , easeFunc( &_animation::_linear::ease ) , fromValue( from ) , toValue( to ) , runs( false )
		{ }
		
		//! Set Address of the Variable to write
		void setter( _u32* destination ){ this->destination = destination; }
		
		void setter( function<void(T)> setterFunc ){ this->setterFunc = setterFunc; }
		
		//! Start the animation
		void start( _u32 timing ){ this->deltaValue = this->toValue - this->fromValue; this->runs = true; this->startTime = timing; }
		
		//! Set fromValue
		void setFromValue( T fromValue ){
			this->fromValue = fromValue;
		}
		
		//! setToValue
		void setToValue( T toValue ){
			this->toValue = toValue;
		}
		
		//! Apply the Value
		void step( _u32 timing )
		{
			if( !this->runs )
				return;
				
			_u32 tElapsed = timing - this->startTime;
			
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
				T value = this->easeFunc( tElapsed , this->fromValue , this->deltaValue , this->duration );
				if( this->setterFunc != nullptr )
					this->setterFunc(value);
				if( this->destination != nullptr )
					*this->destination = value;
			}
		}
		
		void setEasing( T (*easeFunc)( float t , float b , float c , float d ) ){
			this->easeFunc = easeFunc;
		}
		
		//! Check if animation is finished
		bool finished( _u32 timing ){ return timing - this->startTime > this->duration; }
		
		class _linear{
			public:
			static T ease( float t , float b , float c , float d ){ return c*t/d + b; }
		};
		
		class _quad{
			public:
			static T easeIn( float t , float b , float c , float d ){ t/=d; return c*t*t + b; }
			static T easeOut( float t , float b , float c , float d ){ t/=d; return -c*t*(t-2) + b; }
			static T easeInOut( float t , float b , float c , float d ){ if ((t/=d/2) < 1) return ((c/2)*(t*t)) + b; return -c/2 * (((t-2)*(--t)) - 1) + b; }
		};
		
		class _sinus{
			public:
			static T easeIn( float t , float b , float c , float d ){ return -c * cos(t/d * (M_PI/2)) + c + b; }
			static T easeOut( float t , float b , float c , float d ){ return c * sin(t/d * (M_PI/2)) + b; }
			static T easeInOut( float t , float b , float c , float d ){ return -c/2 * (cos(M_PI*t/d) - 1) + b; }
		};
		
		class _bounce{
			public:
			static T easeIn( float t , float b , float c , float d ){ return c - easeOut (d-t, 0, c, d) + b; }
			static T easeOut( float t , float b , float c , float d ){ 
				if ((t/=d) < (1/2.75f)) {
					return c*(7.5625f*t*t) + b;
				} else if (t < (2/2.75f)) {
					float postFix = t-=(1.5f/2.75f);
					return c*(7.5625f*(postFix)*t + .75f) + b;
				} else if (t < (2.5/2.75)) {
					float postFix = t-=(2.25f/2.75f);
					return c*(7.5625f*(postFix)*t + .9375f) + b;
				} else {
					float postFix = t-=(2.625f/2.75f);
					return c*(7.5625f*(postFix)*t + .984375f) + b; 
				}
			}
			static T easeInOut( float t , float b , float c , float d ){ if (t < d/2) return easeIn (t*2, 0, c, d) * .5f + b; else return easeOut (t*2-d, 0, c, d) * .5f + c*.5f + b; }
		};
		
		class _back{
			public:
			static T easeIn( float t , float b , float c , float d ){ float s = 1.70158f; float postFix = t/=d; return c*(postFix)*t*((s+1)*t - s) + b; }
			static T easeOut( float t , float b , float c , float d ){ float s = 1.70158f; return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b; }
			static T easeInOut( float t , float b , float c , float d ){ float s = 1.70158f; if ((t/=d/2) < 1) return c/2*(t*t*(((s*=(1.525f))+1)*t - s)) + b; float postFix = t-=2; return c/2*((postFix)*t*(((s*=(1.525f))+1)*t + s) + 2) + b; }
		};
		
		class _circular{
			public:
			static T easeIn( float t , float b , float c , float d ){ return -c * (sqrt(1 - (t/=d)*t) - 1) + b; }
			static T easeOut( float t , float b , float c , float d ){ return c * sqrt(1 - (t=t/d-1)*t) + b; }
			static T easeInOut( float t , float b , float c , float d ){ if ((t/=d/2) < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b; return c/2 * (sqrt(1 - t*(t-=2)) + 1) + b; }
		};
		
		class _cubic{
			public:
			static T easeIn( float t , float b , float c , float d ){ return c*(t/=d)*t*t + b; }
			static T easeOut( float t , float b , float c , float d ){ return c*((t=t/d-1)*t*t + 1) + b; }
			static T easeInOut( float t , float b , float c , float d ){ if ((t/=d/2) < 1) return c/2*t*t*t + b; return c/2*((t-=2)*t*t + 2) + b; }
		};
		
		class _elastic{
			public:
			static T easeIn( float t , float b , float c , float d ){ 
				if (t==0)
					return b; 
				if ((t/=d)==1)
					return b+c;  
				float p=d*.3f;
				float a=c; 
				float s=p/4;
				float postFix =a*pow(2,10*(t-=1)); // this is a fix, again, with post-increment operators
				return -(postFix * sin((t*d-s)*(2*M_PI)/p )) + b;
			}
			static T easeOut( float t , float b , float c , float d ){ 
				if (t==0)
					return b;  
				if ((t/=d)==1)
					return b+c;  
				float p=d*.3f;
				float a=c; 
				float s=p/4;
				return (a*pow(2,-10*t) * sin( (t*d-s)*(2*M_PI)/p ) + c + b);
			}
			static T easeInOut( float t , float b , float c , float d ){
				if (t==0)
					return b; 
				if ((t/=d/2)==2)
					return b+c; 
				float p=d*(.3f*1.5f);
				float a=c; 
				float s=p/4;
				 
				if (t < 1) {
					float postFix =a*pow(2,10*(t-=1)); // postIncrement is evil
					return -.5f*(postFix* sin( (t*d-s)*(2*M_PI)/p )) + b;
				} 
				float postFix =  a*pow(2,-10*(t-=1)); // postIncrement is evil
				return postFix * sin( (t*d-s)*(2*M_PI)/p )*.5f + c + b;
			}
		};
		class _expo{
			public:
			static T easeIn( float t , float b , float c , float d ){ return (t==0) ? b : c * pow(2, 10 * (t/d - 1)) + b; }
			static T easeOut( float t , float b , float c , float d ){ return (t==d) ? b+c : c * (-pow(2, -10 * t/d) + 1) + b; }
			static T easeInOut( float t , float b , float c , float d ){ if (t==0) return b; if (t==d) return b+c; if ((t/=d/2) < 1) return c/2 * pow(2, 10 * (t - 1)) + b; return c/2 * (-pow(2, -10 * --t) + 2) + b; }
		};
};

template<typename T>
class _animationsGroup{
	
	deque<_animation<T>*>	animations;
	
	public:
	
		void step();
		
		void add( _animation<T>* anim );
};

#endif