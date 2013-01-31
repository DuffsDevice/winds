#include "_type/type.rect.h"

_area& _area::toRelative( const _coord absX , const _coord absY )
{
	for( _rect &rc : t_rects )
		rc.toRelative( absX , absY );
	return *this;
}

_area& _area::clipToIntersect( const _rect limits )
{
	// Temp Rects
	_list<_rect> tR = t_rects;
	
	t_rects.clear();
	
	for( _rect& rc : tR )
		add( rc.clipToIntersect( limits ) );
	
	return *this;
}

_area& _area::reduce( const _rect& dim )
{
	// Temp Rects
	_list<_rect> tR = t_rects;
	
	// Clear
	t_rects.clear();
	
	for( _rect& rc : tR )
		for( const _rect& t : rc.reduce( dim ) )
			t_rects.push_back( t );
	
	return *this;
}