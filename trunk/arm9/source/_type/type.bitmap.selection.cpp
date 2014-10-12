#include <_type/type.bitmap.selection.h>

_bitmapSelection _bitmapSelection::fromColorRange( const _bitmap& origin , _color color , _u8 tolerance )
{
	_bitmapSelection selection{ origin.getWidth() , origin.getHeight() };
	
	// Pointer to the memory of the bitmap
	_pixelArray ptr = origin.getBitmap();
	_pixel pixelValue = color.getColor();
	
	if( color.getAlpha() ){
		for( auto flag : selection.data )
			if( _color(*ptr++).getAlpha() )
				flag = true;
	}else if( tolerance == 0 ){
		for( auto flag : selection.data )
			if( *ptr++ == pixelValue )
				flag = true;
	}else if( tolerance == 54 ){
		for( auto flag : selection.data )
			flag = true;
	}else{
		for( auto flag : selection.data )
			if( _color::distance( _color(*ptr++) , color , true ) <= tolerance )
				flag = true;
	}
	
	return move(selection);
}

_bitmapSelection _bitmapSelection::fromBrightnessRange( const _bitmap& origin , _u8 brightness , _u8 tolerance )
{
	_bitmapSelection selection{ origin.getWidth() , origin.getHeight() };
	
	// Pointer to the memory of the bitmap
	_pixelArray ptr = origin.getBitmap();
	_pixel black = _color::black.getColor();
	_pixel white = _color::white.getColor();
	
	if( tolerance == 0 ){
		if( brightness == 0 ){
			for( auto flag : selection.data )
				if( *ptr++ == black )
					flag = true;
		}
		else if( brightness == 100 ){
			for( auto flag : selection.data )
				if( *ptr++ == white )
					flag = true;
		}else{
			for( auto flag : selection.data )
				if( _color(*ptr++).getL() == brightness )
					flag = true;
		}
	}else if( tolerance == 100 ){
		for( auto flag : selection.data )
			flag = true;
	}else{
		for( auto flag : selection.data ){
			_s8 difference = _s8(_color(*ptr++).getL()) - _s8(brightness);
			if( abs( difference ) <= tolerance )
				flag = true;
		}
	}
	
	return move(selection);
}