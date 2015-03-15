#ifndef _WIN_T_ANIMATION_
#define _WIN_T_ANIMATION_

#include <_type/type.h>
#include <_type/type.uniqueptr.h>
#include <_type/type.callback.h>
#include <_type/type.paramalloc.h>

typedef _float						(_easingFunction)( _float t , _float b , _float c , _float d );

// t = elapsed Time since Start
// b = startValue
// c = deltaValue
// d = duration of the whole effect
class _animation
{
	private:
	
		_tempTime					startTime;
		_tempTime					duration; //! In Milliseconds
		
		//! Additionally: call a setter function
		_uniquePtr<_intSetCallback>	setterFunc;
		_uniquePtr<_intSetCallback>	finishFunc;
		
		_easingFunction*			easeFunc;
		
		_int						fromValue;
		_int						toValue;
		_int						deltaValue;
		
		_u8							runs;
		
		//! Performes one frame of the animation
		void step();
		
		friend class _animationController;
		
	public:
	
		//! Ctor
		_animation( _s32 from , _s32 to , _tempTime dur );
		
		//! Config
		_animation( const _animation& ) = delete;
		_animation( _animation&& ) = default;
		_animation& operator=( const _animation& ) = delete;
		_animation& operator=( _animation&& ) = default;
		
		//! Dtor
		~_animation(){ this->terminate(); }
		
		//! Set a callback to be the setter
		void setSetter( _paramAlloc<_intSetCallback> setterFunc ){
			this->setterFunc = setterFunc;
		}
		
		//! Set a callback to be called at the end of the animation
		void setFinisher( _paramAlloc<_intSetCallback> finishFunc ){
			this->finishFunc = finishFunc;
		}
		
		//! Remove the currently bound setter-callback
		void deleteSetter(){
			this->setterFunc = nullptr;
		}
		
		//! Remove the currently bound finisher-callback
		void deleteFinisher(){
			this->finishFunc = nullptr;
		}
		
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

extern _fromStr<_easingFunction*>	string2easingFunc;
extern _toStr<_easingFunction*>		easingFunc2string;

#endif