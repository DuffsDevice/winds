#include <_type/type.direction.h>

_fromStr<_direction> string2direction = {
	{ "center" , _direction::center } ,
	{ "middle" , _direction::middle } ,
	{ "left" , _direction::left } ,
	{ "leftup" , _direction::leftup } ,
	{ "leftdown" , _direction::leftdown } ,
	{ "up" , _direction::up } ,
	{ "upleft" , _direction::upleft } ,
	{ "upright" , _direction::upright } ,
	{ "down" , _direction::down } ,
	{ "downright" , _direction::downright } ,
	{ "downleft" , _direction::downleft } ,
	{ "right" , _direction::right } ,
	{ "rightup" , _direction::rightup } ,
	{ "rightdown" , _direction::rightdown } ,
};
_toStr<_direction> direction2string = {
	{ _direction::center , "center" } ,
	{ _direction::middle , "middle" } ,
	{ _direction::left , "left" } ,
	{ _direction::leftup , "leftup" } ,
	{ _direction::leftdown , "leftdown" } ,
	{ _direction::up , "up" } ,
	{ _direction::upleft , "upleft" } ,
	{ _direction::upright , "upright" } ,
	{ _direction::down , "down" } ,
	{ _direction::downright , "downright" } ,
	{ _direction::downleft , "downleft" } ,
	{ _direction::right , "right" } ,
	{ _direction::rightup , "rightup" } ,
	{ _direction::rightdown , "rightdown" } ,
};