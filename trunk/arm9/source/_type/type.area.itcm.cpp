#include "_type/type.rect.h"

__attribute__((hot)) _area& _area::clipToIntersect( const _rect& limits )
{
	_codeAnalyzer analyzer {"_rect::clipToIntersect"};
	
	// Fastest way!!!
	t_rects.erase(
		remove_if(
			t_rects.begin()
			, t_rects.end()
			, [=]( _rect& rc )->bool{
				rc.clipToIntersect( limits );
				return !rc.isValid();
			}
		)
		, t_rects.end()
	);
	
	return *this;
}

__attribute__((hot)) _area& _area::reduce( const _rect& dim )
{
	_codeAnalyzer analyzer {"_rect::reduce"};
	
	// Temp Rects
	_vector<_rect> tR = move( t_rects );
	
	for( _rect& rc : tR )
		this->add( rc.reduce( dim ) );
	
	return *this;
}

__attribute__((hot)) _area& _area::toRelative( const _coord absX , const _coord absY )
{
	_codeAnalyzer analyzer {"_rect::toRelative"};
	
	for( _rect &rc : t_rects )
		rc.toRelative( absX , absY );
	return *this;
}

__attribute__((hot)) _area _area::toRelative( const _coord absX , const _coord absY ) const
{
	_codeAnalyzer analyzer {"_rect::toRelative (const)"};
	
	_vector<_rect> tR;
	
	for( const _rect &rc : t_rects )
		tR.emplace_back( rc.x - absX , rc.y - absY , rc.width , rc.height );
	
	return _area( move(tR) );
}