#include <_type/type.bitmap.toner.h>

void _bitmapToner::convertToGreyscale() const
{
	// Pointer to the data
	_pixelArray data = this->base.getBitmap();
	
	for( _u32 i = this->base.getWidth() * this->base.getHeight() ; i-- > 0 ; ){
		_color cur = *data;
		if( cur.getAlpha() )
			*data = _color::fromBW( cur.getL() * 31 / 100  );
		++data;
	}
}

bool _bitmapToner::convertToGreyscale( const _bitmapSelection& selection ) const 
{
	if( selection.getWidth() != this->base.getWidth() || selection.getHeight() != this->base.getWidth() )
		return false;
	
	// Pointer to the data
	_pixelArray data = this->base.getBitmap();
	
	// Pointer to the selection data
	const _bitField& selectionData = selection.data;
	
	// Iterate through pixels
	for( bool selected : selectionData ){
		_color cur = *data;
		if( selected & cur.getAlpha() )
			*data = _color::fromBW( cur.getL() * 31 / 100 );
		++data;
	}
	
	return true;
}

bool _bitmapToner::fill( const _bitmapSelection& selection , _color replacement ) const 
{
	if( selection.getWidth() != this->base.getWidth() || selection.getHeight() != this->base.getWidth() )
		return false;
	
	// Pointer to the data
	_pixelArray data = this->base.getBitmap();
	
	// Pointer to the selection data
	const _bitField& selectionData = selection.data;
	
	// Iterate through pixels
	for( bool selected : selectionData ){
		if( selected )
			*data = replacement;
		++data;
	}
	
	return true;
}

void _bitmapToner::invert() const
{
	// Pointer to the data
	_pixelArray data = this->base.getBitmap();
	
	for( _u32 i = this->base.getWidth() * this->base.getHeight() ; i-- > 0 ; ){
		*data = _color(*data).invert();
		++data;
	}
}

bool _bitmapToner::invert( const _bitmapSelection& selection ) const 
{
	if( selection.getWidth() != this->base.getWidth() || selection.getHeight() != this->base.getWidth() )
		return false;
	
	// Pointer to the data
	_pixelArray data = this->base.getBitmap();
	
	// Pointer to the selection data
	const _bitField& selectionData = selection.data;
	
	// Iterate through pixels
	for( bool selected : selectionData ){
		if( selected )
			*data = _color(*data).invert();
		++data;
	}
	
	return true;
}

void _bitmapToner::convertBrightnessToPalette( _colorPalette palette , bool extendedRange ) const
{
	// Pointer to the data
	_pixelArray data = this->base.getBitmap();
	
	for( _u32 i = this->base.getWidth() * this->base.getHeight() ; i-- > 0 ; ){
		_color cur = *data;
		if( cur.getAlpha() )
			*data = palette[ extendedRange ? cur.getL() : cur.getL() * 31 / 100 ];
		++data;
	}
}

bool _bitmapToner::convertBrightnessToPalette( _colorPalette palette , const _bitmapSelection& selection , bool extendedRange ) const 
{
	if( selection.getWidth() != this->base.getWidth() || selection.getHeight() != this->base.getWidth() )
		return false;
	
	// Pointer to the data
	_pixelArray data = this->base.getBitmap();
	
	// Pointer to the selection data
	const _bitField& selectionData = selection.data;
	
	// Iterate through pixels
	for( bool selected : selectionData ){
		_color cur = *data;
		if( selected & cur.getAlpha() )
			*data = palette[ extendedRange ? cur.getL() : cur.getL() * 31 / 100 ];
		++data;
	}
	
	return true;
}