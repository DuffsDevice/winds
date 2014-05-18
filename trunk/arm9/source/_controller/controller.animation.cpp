#include "_controller/controller.animation.h"
#include <algorithm>
using namespace std::placeholders;

void _animationController::addAnimationToExecute( _animation* anim )
{
	// Add the animation to the list only if its not already present
	if(
		std::find( runningAnimations.begin() , runningAnimations.end() , anim ) == runningAnimations.end()
		&& std::find( animationsToExecute.begin() , animationsToExecute.end() , anim ) == animationsToExecute.end()
	)
		animationsToExecute.push_back( anim );
};

void _animationController::end()
{
	std::for_each(
		_animationController::runningAnimations.begin()
		, _animationController::runningAnimations.end()
		, std::bind( &_animation::terminate , _1 , false )
	);
	std::for_each(
		_animationController::animationsToExecute.begin()
		, _animationController::animationsToExecute.end()
		, std::bind( &_animation::terminate , _1 , false )
	);
	
	// Clear buffer
	_animationController::animationsToExecute.clear();
	_animationController::animationsToExecute.clear();
}

void _animationController::frame()
{
	// Move timers to execute
	move( _animationController::animationsToExecute.begin() , _animationController::animationsToExecute.end() , std::back_inserter( _animationController::runningAnimations ) );
	
	// Clear buffer
	_animationController::animationsToExecute.clear();
	
	// Iterate through all running runningAnimations
	_animationController::runningAnimations.erase(
		remove_if(
			_animationController::runningAnimations.begin()
			, _animationController::runningAnimations.end()
			, []( _animation* anim )->bool
			{
				anim->step();
				return !anim->runs;
			}
		)
		, _animationController::runningAnimations.end()
	);
}

_animationList _animationController::animationsToExecute;
_animationList _animationController::runningAnimations;