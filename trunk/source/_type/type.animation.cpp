#include "_type/type.animation.h"
#include "_type/type.system.h"

#include <cmath>
#define  M_PI        3.14159265358979323846

_animation::_animation( int from , int to , _u32 dur ) :
	startTime( 0 ) , duration( dur ) , destination( nullptr ) , setterFunc( nullptr ) , finishFunc( nullptr ) , easeFunc( &_animation::_linear::ease ) , fromValue( from ) , toValue( to ) , runs( false )
{ }

_animation::~_animation()
{
	this->terminate();
}

void _animation::start(){
	this->deltaValue = this->toValue - this->fromValue;
	this->runs = true;
	this->startTime = _system_->getTime();
	_system_->executeAnimation( this );
}

void _animation::setter( _s32* destination )
{	
	this->destination = destination;
}

void _animation::terminate()
{
	if( this->runs )
	{
		_system_->terminateAnimation( this );
		this->runs = false;
	}
}

void _animation::step( _u32 curTime )
{
	if( !this->runs )
		return;
		
	_u32 tElapsed = curTime - this->startTime;
	
	if( tElapsed > this->duration )
	{
		this->runs = false;
		//printf("end, %d,%p\n", this->finished(), this );
		if( this->setterFunc != nullptr )
			this->setterFunc( this->toValue );
		if( this->destination != nullptr )
			*this->destination = this->toValue;
		if( this->finishFunc )
				this->finishFunc( this->toValue );
		return;
	}
	if( this->easeFunc != nullptr )
	{
		int value = this->easeFunc( tElapsed , this->fromValue , this->deltaValue , this->duration ) + 0.5;
		if( this->setterFunc != nullptr )
			this->setterFunc(value);
		if( this->destination != nullptr )
			*this->destination = value;
	}
}


_float _animation::_linear::ease( _float t , _float b , _float c , _float d ){ return c*t/d + b; }

_float _animation::_quad::easeIn( _float t , _float b , _float c , _float d ){ t/=d; return c*t*t + b; }
_float _animation::_quad::easeOut( _float t , _float b , _float c , _float d ){ t/=d; return -c*t*(t-2) + b; }
_float _animation::_quad::easeInOut( _float t , _float b , _float c , _float d ){ if ((t/=d/2) < 1) return ((c/2)*(t*t)) + b; return -c/2 * (((t-2)*(--t)) - 1) + b; }

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
_float _animation::_back::easeOut( _float t , _float b , _float c , _float d ){ _float s = 1.70158f; return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b; }
_float _animation::_back::easeInOut( _float t , _float b , _float c , _float d ){ _float s = 1.70158f; if ((t/=d/2) < 1) return c/2*(t*t*(((s*=(1.525f))+1)*t - s)) + b; _float postFix = t-=2; return c/2*((postFix)*t*(((s*=(1.525f))+1)*t + s) + 2) + b; }

_float _animation::_circular::easeIn( _float t , _float b , _float c , _float d ){ return -c * (sqrt(1 - (t/=d)*t) - 1) + b; }
_float _animation::_circular::easeOut( _float t , _float b , _float c , _float d ){ return c * sqrt(1 - (t=t/d-1)*t) + b; }
_float _animation::_circular::easeInOut( _float t , _float b , _float c , _float d ){ if ((t/=d/2) < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b; return c/2 * (sqrt(1 - t*(t-=2)) + 1) + b; }

_float _animation::_cubic::easeIn( _float t , _float b , _float c , _float d ){ return c*(t/=d)*t*t + b; }
_float _animation::_cubic::easeOut( _float t , _float b , _float c , _float d ){ return c*((t=t/d-1)*t*t + 1) + b; }
_float _animation::_cubic::easeInOut( _float t , _float b , _float c , _float d ){ if ((t/=d/2) < 1) return c/2*t*t*t + b; return c/2*((t-=2)*t*t + 2) + b; }

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
