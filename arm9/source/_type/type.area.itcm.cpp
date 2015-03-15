#include <_type/type.rect.h>

optimized _area& _area::clipToIntersect( const _rect& limits )
{
	_codeAnalyzer analyzer {"_rect::clipToIntersect"};
	
	if( !limits.isValid() )
		this->rects.clear();
	
	// Fastest way!!!
	this->rects.erase(
		remove_if(
			this->rects.begin()
			, this->rects.end()
			, [&limits]( _rect& rc )->bool{
				rc.clipToIntersect( limits );
				return !rc.isValid();
			}
		)
		, this->rects.end()
	);
	
	return *this;
}

optimized _area& _area::reduce( const _rect& dim )
{
	_codeAnalyzer analyzer {"_rect::reduce"};
	
	if( !dim.isValid() )
		return *this;
	
	// Temp Rects
	_vector<_rect> tR = move( this->rects );
	
	for( _rect& rc : tR )
		this->add( rc.reduce( dim ) );
	
	return *this;
}

optimized _area& _area::toRelative( const _coord absX , const _coord absY )
{
	_codeAnalyzer analyzer {"_rect::toRelative"};
	
	for( _rect &rc : this->rects )
		rc.toRelative( absX , absY );
	return *this;
}

optimized _area _area::toRelative( const _coord absX , const _coord absY ) const
{
	_codeAnalyzer analyzer {"_rect::toRelative (const)"};
	
	_vector<_rect> tR;
	
	for( const _rect &rc : this->rects )
		tR.emplace_back( rc.x - absX , rc.y - absY , rc.width , rc.height );
	
	return _area( move(tR) );
}