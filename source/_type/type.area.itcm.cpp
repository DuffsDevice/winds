#include "_type/type.rect.h"

_area& _area::clipToIntersect( const _rect limits )
{
	// Temp Rects
	_list<_rect> tR = move( t_rects );
	
	for( _rect& rc : tR )
		add( rc.clipToIntersect( limits ) );
	
	return *this;
}

_area& _area::reduce( const _rect& dim )
{
	// Temp Rects
	_list<_rect> tR = move( t_rects );
	
	for( _rect& rc : tR )
		for( const _rect& t : rc.reduce( dim ) )
			t_rects.push_back( t );
	
	return *this;
}