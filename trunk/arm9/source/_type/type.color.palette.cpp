#include <_type/type.color.palette.h>
#include <_library/library.color.mediancut.h>

_colorPalette::_colorPalette( _initializerList<_color> list ) :
	hasTransparent( false )
{
	colors.reserve( list.size() );
	
	_u32 idx = 0;
	for( _color col : list ){
		if( !col.getAlpha() )
			colors[idx++] = col;
		else
			this->hasTransparent = true;
	}
}

_u32 _colorPalette::getClosestColor( _color color ) const
{
	_u32 numColors = this->colors.size();
	
	if( !color.getAlpha() )
		return numColors * this->hasTransparent;
	
	_u32 bestIdx = 0;
	_u32 smallestDistance = ~0;
	for( _u32 idx = 0 ; idx < numColors ; idx++ )
	{
		_u32 newDistance = _color::distance( this->colors[idx] , color , true );
		if( newDistance < smallestDistance ){
			smallestDistance = newDistance;
			bestIdx = idx;
		}
	}
	return bestIdx;
}

_colorPalette _colorPalette::fromBitmap( const _bitmap& bmp )
{
	_map<_pixel,_u8> palette;
	_colorPalette result;
	
	_s32 idx = bmp.getWidth() * bmp.getHeight();
	while( --idx >= 0 )
	{
		_color col = bmp[idx]; // Get nth color in bitmap
		
		if( !col.getAlpha() ){ // skip transparent colors
			result.hasTransparent = true;
			continue;
		}
		
		palette[col.getColor()]; // Creates entries for each existing color
	}
	
	// Reserve space
	result.colors.resize( palette.size() );
	
	idx = 0;
	for( _pair<_color,_u8> col : palette )
		result.colors[idx++] = col.first;
	
	return result;
}

void _colorPalette::downsample( _u32 numberOfColors )
{
	if( this->hasTransparent )
		numberOfColors--;
	
	if( numberOfColors >= this->colors.size() )
		return;
	
	_u8* buffer = new _u8[ this->colors.size() * 3 ];
	_u8* tempBuffer = buffer;
	for( _color col : this->colors )
	{
		tempBuffer[0] = col.getR()<<3;
		tempBuffer[1] = col.getG()<<3;
		tempBuffer[2] = col.getB()<<3;
		tempBuffer += 3;
	}
	
	std::vector<Point> result = medianCut( (Point*) buffer , this->colors.size() , numberOfColors );
	
	this->colors.resize( result.size() + this->hasTransparent );
	
	_u32 idx = 0;
	for( Point pt : result )
		this->colors[idx] = _color::fromRGB8( pt.x[0] , pt.x[1] , pt.x[2] );
}

void _colorPalette::addColor( _color col , bool noDuplicates ){
	if( noDuplicates && find( this->colors.begin() , this->colors.end() , col ) != this->colors.end() )
		return;
	this->colors.push_back( col );
}