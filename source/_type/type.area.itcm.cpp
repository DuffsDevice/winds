#include "_type/type.rect.h"

_area& _area::clipToIntersect( const _rect& limits )
{
	// Fastest way!!!
	t_rects.remove_if(
		[=]( _rect& rc )->bool{
			if( !rc.intersectsWith( limits ) )
				return true;
			rc.clipToIntersect( limits );
			return false;
		}
	);
	
	return *this;
}

_area& _area::reduce( const _rect& dim )
{
	// Temp Rects
	_list<_rect> tR = move( t_rects );
	
	for( _rect& rc : tR )
		add( rc.reduce( dim ) );
	
	return *this;
}

_area& _area::toRelative( const _coord absX , const _coord absY )
{
	for( _rect &rc : t_rects )
		rc.toRelative( absX , absY );
	return *this;
}