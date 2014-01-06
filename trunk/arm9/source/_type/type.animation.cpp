#include "_type/type.animation.h"
#include "_type/type.system.h"

#include <cmath>
#define  M_PI        3.14159265358979323846

_animationList _animation::globalAnimations;
_animationList _animation::globalAnimationsToExecute;

_animation::_animation( int from , int to , _tempTime dur ) :
	startTime( 0 ) , duration( dur ) , setterFunc( nullptr ) , finishFunc( nullptr ) , easeFunc( &_animation::_linear::ease ) , fromValue( from ) , toValue( to ) , runs( false )
{ }

void _animation::start()
{
	this->deltaValue = this->toValue - this->fromValue;
	
	// If it was paused instead of terminated
	// and if it is not currently running: continue our work!
	if(	this->startTime && !this->runs )
		this->startTime = _system::getHighResTime() - this->startTime; // startTime determines the already elapsed milliseconds
	else
		this->startTime = _system::getHighResTime();
	
	this->runs = true;
	
	// Add the animation to the list of running animations
	if(
		find( _animation::globalAnimations.begin() , _animation::globalAnimations.end() , this ) == _animation::globalAnimations.end()
		&& find( _animation::globalAnimationsToExecute.begin() , _animation::globalAnimationsToExecute.end() , this ) == _animation::globalAnimationsToExecute.end()
	)
		_animation::globalAnimationsToExecute.push_back( this );
}

void _animation::runAnimations()
{
	// Move timers to execute
	move( _animation::globalAnimationsToExecute.begin() , _animation::globalAnimationsToExecute.end() , std::back_inserter( _animation::globalAnimations ) );
	
	// Clear buffer
	globalAnimationsToExecute.clear();
	
	// Iterate through all running animations
	_animation::globalAnimations.erase(
		remove_if(
			_animation::globalAnimations.begin()
			, _animation::globalAnimations.end()
			, []( _animation* anim )->bool
			{
				anim->step(); return !anim->isRunning();
			}
		)
		, _animation::globalAnimations.end()
	);
}

void _animation::pause()
{
	if( !this->runs )
		return;
	
	this->runs = false;
	
	// Store elapsed time for using start() again
	this->startTime = _system::getHighResTime() - this->startTime;
}

void _animation::terminate( bool animToEnd )
{
	if( !this->runs )
		return;
	
	this->runs = false;
	
	this->startTime = 0;
	
	// Animate 'till end?
	if( animToEnd )
	{
		if( this->setterFunc )
			(*this->setterFunc)( this->toValue );
		
		if( this->finishFunc )
			(*this->finishFunc)( this->toValue );
	}
}

void _animation::step()
{
	if( !this->runs )
		return;
	
	_tempTime curTime = _system::getHighResTime();
	
	_u32 tElapsed = curTime - this->startTime;
	
	if( tElapsed > this->duration )
	{
		this->runs = false;
		this->startTime = 0;
		if( this->setterFunc )
			(*this->setterFunc)( this->toValue );
		if( this->finishFunc )
			(*this->finishFunc)( this->toValue );
		return;
	}
	
	// Has valid easing func?
	if( this->easeFunc )
	{
		int value = this->easeFunc( tElapsed , this->fromValue , this->deltaValue , this->duration ) + 0.5;
		if( this->setterFunc != nullptr )
			(*this->setterFunc)(value);
	}
}


// Thanks to penner easing for c++
_float _animation::_linear::ease( _float t , _float b , _float c , _float d ){ return c*t/d + b; }

_float _animation::_quad::easeIn( _float t , _float b , _float c , _float d ){ t/=d; return c*t*t + b; }
_float _animation::_quad::easeOut( _float t , _float b , _float c , _float d ){ t/=d; return -c*t*(t-2) + b; }
_float _animation::_quad::easeInOut( _float t , _float b , _float c , _float d ){ t/=d/2; if (t < 1) return ((c/2)*(t*t)) + b; return -c/2 * (((t-2)*(t-1)) - 1) + b; }

_float _animation::_sinus::easeIn( _float t , _float b , _float c , _float d ){ return -c * cos(t/d * (M_PI/2)) + c + b; }
_float _animation::_sinus::easeOut( _float t , _float b , _float c , _float d ){ return c * sin(t/d * (M_PI/2)) + b; }
_float _animation::_sinus::easeInOut( _float t , _float b , _float c , _float d ){ return -c/2 * (cos(M_PI*t/d) - 1) + b; }

_float _animation::_bounce::easeIn( _float t , _float b , _float c , _float d ){ return c - easeOut (d-t, 0, c, d) + b; }
_float _animation::_bounce::easeOut( _float t , _float b , _float c , _float d ){ 
	if ((t/=d) < (1/2.75f)) {
		return c*(7.5625f*t*t) + b;
	} else if (t < (2/2.75f)) {
		_float postFix = t-=(1.5f/2.75f);
		return c*(7.5625f*(postFix)*t + .75f) + b;
	} else if (t < (2.5/2.75)) {
		_float postFix = t-=(2.25f/2.75f);
		return c*(7.5625f*(postFix)*t + .9375f) + b;
	} else {
		_float postFix = t-=(2.625f/2.75f);
		return c*(7.5625f*(postFix)*t + .984375f) + b; 
	}
}
_float _animation::_bounce::easeInOut( _float t , _float b , _float c , _float d ){ if (t < d/2) return easeIn (t*2, 0, c, d) * .5f + b; else return easeOut (t*2-d, 0, c, d) * .5f + c*.5f + b; }

_float _animation::_back::easeIn( _float t , _float b , _float c , _float d ){ _float s = 1.70158f; _float postFix = t/=d; return c*(postFix)*t*((s+1)*t - s) + b; }
_float _animation::_back::easeOut( _float t , _float b , _float c , _float d ){ t=t/d-1; _float s = 1.70158f; return c*(t*t*((s+1)*t + s) + 1) + b; }
_float _animation::_back::easeInOut( _float t , _float b , _float c , _float d ){ t/=d/2; _float s = 1.70158f * 1.525f; if (t < 1) return c/2*(t*t*((s+1)*t - s)) + b; _float postFix = t-=2; return c/2*((postFix)*t*((s+1)*t + s) + 2) + b; }

_float _animation::_circular::easeIn( _float t , _float b , _float c , _float d ){ t/=d; return -c * (sqrt(1 - t*t) - 1) + b; }
_float _animation::_circular::easeOut( _float t , _float b , _float c , _float d ){ t=t/d-1; return c * sqrt(1 - t*t) + b; }
_float _animation::_circular::easeInOut( _float t , _float b , _float c , _float d ){ t/=d/2; if (t < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b; return c/2 * (sqrt(1 - t*(t-2)) + 1) + b; }

_float _animation::_cubic::easeIn( _float t , _float b , _float c , _float d ){ t/=d; return c*(t)*t*t + b; }
_float _animation::_cubic::easeOut( _float t , _float b , _float c , _float d ){ t=t/d-1; return c*(t*t*t + 1) + b; }
_float _animation::_cubic::easeInOut( _float t , _float b , _float c , _float d ){ t/=d/2; if (t < 1) return c/2*t*t*t + b; t-=2; return c/2*(t*t*t + 2) + b; }

_float _animation::_elastic::easeIn( _float t , _float b , _float c , _float d ){ 
	if (t==0)
		return b; 
	if ((t/=d)==1)
		return b+c;  
	_float p=d*.3f;
	_float a=c; 
	_float s=p/4;
	_float postFix =a*pow(2,10*(t-=1)); // this is a fix, again, with post-increment operators
	return -(postFix * sin((t*d-s)*(2*M_PI)/p )) + b;
}
_float _animation::_elastic::easeOut( _float t , _float b , _float c , _float d ){ 
	if (t==0)
		return b;  
	if ((t/=d)==1)
		return b+c;  
	_float p=d*.3f;
	_float a=c; 
	_float s=p/4;
	return (a*pow(2,-10*t) * sin( (t*d-s)*(2*M_PI)/p ) + c + b);
}
_float _animation::_elastic::easeInOut( _float t , _float b , _float c , _float d ){
	if (t==0)
		return b; 
	if ((t/=d/2)==2)
		return b+c; 
	_float p=d*(.3f*1.5f);
	_float a=c; 
	_float s=p/4;
	 
	if (t < 1) {
		_float postFix =a*pow(2,10*(t-=1)); // postIncrement is evil
		return -.5f*(postFix* sin( (t*d-s)*(2*M_PI)/p )) + b;
	} 
	_float postFix =  a*pow(2,-10*(t-=1)); // postIncrement is evil
	return postFix * sin( (t*d-s)*(2*M_PI)/p )*.5f + c + b;
}

_float _animation::_expo::easeIn( _float t , _float b , _float c , _float d ){ return (t==0) ? b : c * pow(2, 10 * (t/d - 1)) + b; }
_float _animation::_expo::easeOut( _float t , _float b , _float c , _float d ){ 
	return (t==d) ? b+c : c * (-pow(2, -10 * t/d) + 1) + b; 
}
_float _animation::_expo::easeInOut( _float t , _float b , _float c , _float d ){ if (t==0) return b; if (t==d) return b+c; if ((t/=d/2) < 1) return c/2 * pow(2, 10 * (t - 1)) + b; return c/2 * (-pow(2, -10 * --t) + 2) + b; }


_fromStr<_easingFunction*> string2easingFunc = {
	{ "linear:ease" , _animation::_linear::ease },
	
	{ "quad:easeIn" , _animation::_quad::easeIn },
	{ "quad:easeOut" , _animation::_quad::easeOut },
	{ "quad:easeInOut" , _animation::_quad::easeInOut },
	
	{ "sinus:easeIn" , _animation::_sinus::easeIn },
	{ "sinus:easeOut" , _animation::_sinus::easeOut },
	{ "sinus:easeInOut" , _animation::_sinus::easeInOut },
	
	{ "bounce:easeIn" , _animation::_bounce::easeIn },
	{ "bounce:easeOut" , _animation::_bounce::easeOut },
	{ "bounce:easeInOut" , _animation::_bounce::easeInOut },
	
	{ "back:easeIn" , _animation::_back::easeIn },
	{ "back:easeOut" , _animation::_back::easeOut },
	{ "back:easeInOut" , _animation::_back::easeInOut },
	
	{ "circular:easeIn" , _animation::_circular::easeIn },
	{ "circular:easeOut" , _animation::_circular::easeOut },
	{ "circular:easeInOut" , _animation::_circular::easeInOut },
	
	{ "cubic:easeIn" , _animation::_cubic::easeIn },
	{ "cubic:easeOut" , _animation::_cubic::easeOut },
	{ "cubic:easeInOut" , _animation::_cubic::easeInOut },
	
	{ "elastic:easeIn" , _animation::_elastic::easeIn },
	{ "elastic:easeOut" , _animation::_elastic::easeOut },
	{ "elastic:easeInOut" , _animation::_elastic::easeInOut },
	
	{ "expo:easeIn" , _animation::_expo::easeIn },
	{ "expo:easeOut" , _animation::_expo::easeOut },
	{ "expo:easeInOut" , _animation::_expo::easeInOut }
};
_toStr<_easingFunction*> easingFunc2string = {
	{ _animation::_linear::ease , "linear:ease" },
	
	{ _animation::_quad::easeIn , "quad:easeIn" },
	{ _animation::_quad::easeOut , "quad:easeOut" },
	{ _animation::_quad::easeInOut , "quad:easeInOut" },
	
	{ _animation::_sinus::easeIn , "sinus:easeIn" },
	{ _animation::_sinus::easeOut , "sinus:easeOut" },
	{ _animation::_sinus::easeInOut , "sinus:easeInOut" },
	
	{ _animation::_bounce::easeIn , "bounce:easeIn" },
	{ _animation::_bounce::easeOut , "bounce:easeOut" },
	{ _animation::_bounce::easeInOut , "bounce:easeInOut" },
	
	{ _animation::_back::easeIn , "back:easeIn" },
	{ _animation::_back::easeOut , "back:easeOut" },
	{ _animation::_back::easeInOut , "back:easeInOut" },
	
	{ _animation::_circular::easeIn , "circular:easeIn" },
	{ _animation::_circular::easeOut , "circular:easeOut" },
	{ _animation::_circular::easeInOut , "circular:easeInOut" },
	
	{ _animation::_cubic::easeIn , "cubic:easeIn" },
	{ _animation::_cubic::easeOut , "cubic:easeOut" },
	{ _animation::_cubic::easeInOut , "cubic:easeInOut" },
	
	{ _animation::_elastic::easeIn , "elastic:easeIn" },
	{ _animation::_elastic::easeOut , "elastic:easeOut" },
	{ _animation::_elastic::easeInOut , "elastic:easeInOut" },
	
	{ _animation::_expo::easeIn , "expo:easeIn" },
	{ _animation::_expo::easeOut , "expo:easeOut" },
	{ _animation::_expo::easeInOut , "expo:easeInOut" },
};
