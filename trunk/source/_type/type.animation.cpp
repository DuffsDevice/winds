#include "_type/type.animation.h"
#include "_type/type.system.h"

template<typename T>
void _animationsGroup<T>::step(){
	start:
	for( _animation<T>* anim : this->animations ){
		if( anim->runs )
			anim->step( _system_->getNow() );
		else if( anim->finished( _system_->getNow() ) ){
			this->animations.erase( find( this->animations.begin() , this->animations.end() , anim ) );
			goto start;
		}
	}
}

template<typename T>
void _animationsGroup<T>::add( _animation<T>* anim ){
	for( auto i = this->animations.begin(); i != this->animations.end(); i++ ){
		if( *i == anim )
			return;
	}
	this->animations.push_back( anim );
}

// Generate methods for _animationsGroup<int>
template void _animationsGroup<int>::add( _animation<int>* anim );

template void _animationsGroup<int>::step();