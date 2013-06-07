#include "_type/type.rect.h"

_area& _area::clipToIntersect( const _rect& limits )
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_rect::clipToIntersect" );
	#endif
	
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
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_rect::reduce" );
	#endif
	
	// Temp Rects
	_list<_rect> tR = move( t_rects );
	
	for( _rect& rc : tR )
		add( rc.reduce( dim ) );
	
	return *this;
}

_area& _area::toRelative( const _coord absX , const _coord absY )
{
	#ifdef DEBUG_PROFILING
	_codeAnalyzer a =_codeAnalyzer( "_rect::toRelative" );
	#endif
	
	for( _rect &rc : t_rects )
		rc.toRelative( absX , absY );
	return *this;
}