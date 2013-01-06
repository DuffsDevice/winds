#ifndef _WIN_T_ANIMATION_
#define _WIN_T_ANIMATION_

#include <time.h>
#include "_type/type.h"
#include "_type/type.callback.h"

typedef _float (_easingFunction)( _float t , _float b , _float c , _float d );

// t = elapsed Time since Start
// b = startValue
// c = deltaValue
// d = duration of the whole effect
class _animation{
	
	private:
	
		friend class _system;
		_tempTime			startTime;
		_tempTime			duration; //! In Milliseconds
		
		//! Additionally: call a setter function
		const _callback* 	setterFunc;
		const _callback* 	finishFunc;
		
		_easingFunction*	easeFunc;
		
		int					fromValue;
		int					toValue;
		int					deltaValue;
		
		bool				runs;
		
	public:
	
		_animation( _s32 from , _s32 to , _tempTime dur );
		
		~_animation();
		
		//! Set a lamda-expression to be the setter
		void setter( const _callback* setterFunc ){ if( this->setterFunc ) delete this->setterFunc; this->setterFunc = setterFunc; }
		
		//! Set a lamda-expression to be called at the end of the animation
		void finish( const _callback* finishFunc ){ if( this->finishFunc ) delete this->finishFunc; this->finishFunc = finishFunc; }
		
		//! Start the animation
		void start();
		
		//! Terminate the animation (cannot be restarted from the point of termination)
		void terminate();
		
		//! Set fromValue
		void setDuration( _tempTime val ){
			this->duration = val;
		}
		
		//! get fromValue
		_tempTime getDuration(){
			return this->duration;
		}
		
		//! Set fromValue
		void setFromValue( int fromValue ){
			this->fromValue = fromValue;
		}
		
		//! get fromValue
		int getFromValue(){
			return this->fromValue;
		}
		
		//! setToValue
		void setToValue( int toValue ){
			this->toValue = toValue;
		}
		
		//! getToValue
		int getToValue(){
			return this->toValue;
		}
		
		//! Apply the Value
		void step( _tempTime curTime );
		
		//! setEasing
		void setEasing( _easingFunction* easeFunc ){
			this->easeFunc = easeFunc;
		}
		
		//! getEasing
		_easingFunction* getEasing(  ){
			return this->easeFunc;
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

extern map<string,_easingFunction*> string2easingFunc;
extern map<_easingFunction*,string> easingFunc2string;

#endif