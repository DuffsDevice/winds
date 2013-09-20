#ifndef _WIN_T_ANIMATION_
#define _WIN_T_ANIMATION_

#include <time.h>
#include "_type/type.h"
#include "_type/type.callback.h"

typedef _float						(_easingFunction)( _float t , _float b , _float c , _float d );
typedef _vector<class _animation*>	_animationList;

// t = elapsed Time since Start
// b = startValue
// c = deltaValue
// d = duration of the whole effect
class _animation{
	
	private:
	
		friend class _system;
		_tempTime				startTime;
		_tempTime				duration; //! In Milliseconds
		
		//! Additionally: call a setter function
		const _intSetCallback*	setterFunc;
		const _intSetCallback*	finishFunc;
		
		_easingFunction*		easeFunc;
		
		int						fromValue;
		int						toValue;
		int						deltaValue;
		
		bool					runs;
		
		// performes one frame of the animation
		void step();
		
		static _animationList	globalAnimations;
		static _animationList	globalAnimationsToExecute;
		
		// Processes one frame of each running animation
		static void	runAnimations();
		
	public:
	
		//! Ctor
		_animation( _s32 from , _s32 to , _tempTime dur );
		
		//! Dtor
		~_animation();
		
		//! Set a lamda-expression to be the setter
		template<typename T>
		void setter( T&& setterFunc ){ if( this->setterFunc ) delete this->setterFunc; this->setterFunc = new T( move(setterFunc) ); }
		
		//! Set a lamda-expression to be called at the end of the animation
		template<typename T>
		void finish( T&& finishFunc ){ if( this->finishFunc ) delete this->finishFunc; this->finishFunc = new T( move(finishFunc) ); }
		
		//! Start the animation
		void start();
		
		//! Terminate the animation (a further call to start() will restart the animation)
		//! @param animToEnd Whether to animate until the end or only abort animating
		void terminate( bool animToEnd = false );
		
		//! Pause the animation (a further call to start() will unpause the animation)
		void pause();
		
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
		
		//! setEasing
		void setEasing( _easingFunction* easeFunc ){
			this->easeFunc = easeFunc;
		}
		
		//! getEasing
		_easingFunction* getEasing(  ){
			return this->easeFunc;
		}
		
		//! Check if animation is currently running
		bool isRunning(){ return this->runs; }
		
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

extern _map<string,_easingFunction*> string2easingFunc;
extern _map<_easingFunction*,string> easingFunc2string;

#endif