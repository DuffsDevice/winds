#ifndef _WIN_T_DEPENDENCY_
#define _WIN_T_DEPENDENCY_

#include "_type/type.h"

enum _eventType : _u8;

struct _dependencyMap{
	bool resize : 1;
	bool move : 1;
	bool restyle : 1;
	bool visibility : 1;
	bool add : 1;
	bool remove : 1;
	bool focus : 1;
	bool select : 1;
} PACKED ;

struct _dependency
{
	union{
		_u32				sum;
		struct
		{
			_dependencyMap	parent;
			_dependencyMap	child;
			_dependencyMap	pre;
			_dependencyMap	post;
		};
	};
	
	//! Ctor
	_dependency( _u32 sum = 0 ) : sum( sum ) {}
	
	//! Cast to _u32
	operator _u32(){ return sum; }
	
	//! Test if the underlying flag is set or not
	bool get( _eventType type ) const ;
	
	//! Enable or disable dependency of 'type'
	void set( _eventType type , bool value );
};

#endif