#ifndef _WIN_C_ANIMATION_
#define _WIN_C_ANIMATION_

#include <_type/type.controller.h>
#include <_type/type.animation.h>

typedef _vector<_animation*>	_animationList;

class _animationController : public _controller
{
	private:
		
		friend class _animation;
		
		static _animationList		runningAnimations;
		static _animationList		animationsToExecute;
		
		//! Called by the _animation class
		static void addAnimationToExecute( _animation* );
		
	public:
		
		//! Processes one frame of each running animation
		static void	frame();
		
		//! Finish Animations
		static void end();
};

#endif