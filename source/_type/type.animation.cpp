#include "_type/type.animation.h"
#include "_type/type.system.h"

_u32 _animation::idCount = 0;

_animation::_animation( int from , int to , _u32 dur ) :
	startTime( 0 ) , duration( dur ) , destination( nullptr ) , setterFunc( nullptr ) , easeFunc( &_animation::_linear::ease ) , fromValue( from ) , toValue( to ) , runs( false )
{ this->id = this->idCount++; }

void _animation::start(){
	this->deltaValue = this->toValue - this->fromValue;
	this->runs = true;
	this->startTime = _system_->getTime();
	_system_->executeAnimation( *this );
}

void _animation::setter( _u32* destination )
{	
	this->destination = destination;
}


float _animation::_linear::ease( float t , float b , float c , float d ){ return c*t/d + b; }

float _animation::_quad::easeIn( float t , float b , float c , float d ){ t/=d; return c*t*t + b; }
float _animation::_quad::easeOut( float t , float b , float c , float d ){ t/=d; return -c*t*(t-2) + b; }
float _animation::_quad::easeInOut( float t , float b , float c , float d ){ if ((t/=d/2) < 1) return ((c/2)*(t*t)) + b; return -c/2 * (((t-2)*(--t)) - 1) + b; }

float _animation::_sinus::easeIn( float t , float b , float c , float d ){ return -c * cos(t/d * (M_PI/2)) + c + b; }
float _animation::_sinus::easeOut( float t , float b , float c , float d ){ return c * sin(t/d * (M_PI/2)) + b; }
float _animation::_sinus::easeInOut( float t , float b , float c , float d ){ return -c/2 * (cos(M_PI*t/d) - 1) + b; }

float _animation::_bounce::easeIn( float t , float b , float c , float d ){ return c - easeOut (d-t, 0, c, d) + b; }
float _animation::_bounce::easeOut( float t , float b , float c , float d ){ 
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
float _animation::_bounce::easeInOut( float t , float b , float c , float d ){ if (t < d/2) return easeIn (t*2, 0, c, d) * .5f + b; else return easeOut (t*2-d, 0, c, d) * .5f + c*.5f + b; }

float _animation::_back::easeIn( float t , float b , float c , float d ){ float s = 1.70158f; float postFix = t/=d; return c*(postFix)*t*((s+1)*t - s) + b; }
float _animation::_back::easeOut( float t , float b , float c , float d ){ float s = 1.70158f; return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b; }
float _animation::_back::easeInOut( float t , float b , float c , float d ){ float s = 1.70158f; if ((t/=d/2) < 1) return c/2*(t*t*(((s*=(1.525f))+1)*t - s)) + b; float postFix = t-=2; return c/2*((postFix)*t*(((s*=(1.525f))+1)*t + s) + 2) + b; }

float _animation::_circular::easeIn( float t , float b , float c , float d ){ return -c * (sqrt(1 - (t/=d)*t) - 1) + b; }
float _animation::_circular::easeOut( float t , float b , float c , float d ){ return c * sqrt(1 - (t=t/d-1)*t) + b; }
float _animation::_circular::easeInOut( float t , float b , float c , float d ){ if ((t/=d/2) < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b; return c/2 * (sqrt(1 - t*(t-=2)) + 1) + b; }

float _animation::_cubic::easeIn( float t , float b , float c , float d ){ return c*(t/=d)*t*t + b; }
float _animation::_cubic::easeOut( float t , float b , float c , float d ){ return c*((t=t/d-1)*t*t + 1) + b; }
float _animation::_cubic::easeInOut( float t , float b , float c , float d ){ if ((t/=d/2) < 1) return c/2*t*t*t + b; return c/2*((t-=2)*t*t + 2) + b; }

float _animation::_elastic::easeIn( float t , float b , float c , float d ){ 
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
float _animation::_elastic::easeOut( float t , float b , float c , float d ){ 
	if (t==0)
		return b;  
	if ((t/=d)==1)
		return b+c;  
	float p=d*.3f;
	float a=c; 
	float s=p/4;
	return (a*pow(2,-10*t) * sin( (t*d-s)*(2*M_PI)/p ) + c + b);
}
float _animation::_elastic::easeInOut( float t , float b , float c , float d ){
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

float _animation::_expo::easeIn( float t , float b , float c , float d ){ return (t==0) ? b : c * pow(2, 10 * (t/d - 1)) + b; }
float _animation::_expo::easeOut( float t , float b , float c , float d ){ return (t==d) ? b+c : c * (-pow(2, -10 * t/d) + 1) + b; }
float _animation::_expo::easeInOut( float t , float b , float c , float d ){ if (t==0) return b; if (t==d) return b+c; if ((t/=d/2) < 1) return c/2 * pow(2, 10 * (t - 1)) + b; return c/2 * (-pow(2, -10 * --t) + 2) + b; }
