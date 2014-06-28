#ifndef _WIN_T_DIRECTION_
#define _WIN_T_DIRECTION_

#include "_type/type.h"

enum class _direction : _u8
{
	center = 0,
	middle = center,
	left = 4,
	leftup = 5,
	leftdown = 6,
	up = 1,
	upleft = leftup,
	upright = 9,
	down = 2,
	downright = 10,
	downleft = leftdown,
	right = 8,
	rightup = upright,
	rightdown = downright,
	
	// Masks to filter
	horizontalMask = 12,
	verticalMask = 3
};

//! Filter a horizontal/vertical part out of a given direction
static unused constexpr _direction getHorizontalPart( _direction dir ){
	return _direction( _u8(dir) & _u8(_direction::horizontalMask) );
}
static unused constexpr _direction getVerticalPart( _direction dir ){
	return _direction( _u8(dir) & _u8(_direction::verticalMask) );
}

extern _fromStr<_direction>	string2direction;
extern _toStr<_direction>	direction2string;

#endif