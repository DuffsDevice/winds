#include <_type/type.bitmap.selection.h>

_bitmapSelection _bitmapSelection::fromColorRange( const _bitmap& origin , _color color , _u8 tolerance )
{
	_bitmapSelection selection{ origin.getWidth() , origin.getHeight() };
	
	// Pointer to the memory of the bitmap
	_pixelArray ptr = origin.getBitmap();
	_pixelArray startPtr = ptr;
	_pixelArray endPtr = ptr + origin.getWidth() * origin.getHeight();
	_pixel pixelValue = color.getColor();
	
	if( color.getAlpha() ){
		while( ptr < endPtr ){
			if( _color(*ptr).getAlpha() )
				selection.data.set( ptr - startPtr );
			ptr++;
		}
	}else if( tolerance == 0 ){
		while( ptr < endPtr ){
			if( *ptr == pixelValue )
				selection.data.set( ptr - startPtr );
			ptr++;
		}
	}else if( tolerance >= 54 )
		selection.data.set();
	else
		while( ptr < endPtr ){
			if( _color::distance( _color(*ptr) , color , true ) <= tolerance )
				selection.data.set( ptr - startPtr );
			ptr++;
		}
	
	return move(selection);
}

_bitmapSelection _bitmapSelection::fromBrightnessRange( const _bitmap& origin , _u8 brightness , _u8 tolerance )
{
	_bitmapSelection selection{ origin.getWidth() , origin.getHeight() };
	
	// Pointer to the memory of the bitmap
	_pixelArray ptr = origin.getBitmap();
	_pixelArray startPtr = ptr;
	_pixelArray endPtr = ptr + origin.getWidth() * origin.getHeight();
	_pixel black = _color::black.getColor();
	_pixel white = _color::white.getColor();
	
	if( tolerance == 0 ){
		if( brightness == 0 ){
			while( ptr < endPtr ){
				if( *ptr == black )
					selection.data.set( ptr - startPtr );
				ptr++;
			}
		}
		else if( brightness == 100 ){
			while( ptr < endPtr ){
				if( *ptr == white )
					selection.data.set( ptr - startPtr );
				ptr++;
			}
		}else{
			while( ptr < endPtr ){
				if( _color(*ptr).getL() == brightness )
					selection.data.set( ptr - startPtr );
				ptr++;
			}
		}
	}else if( tolerance == 100 )
		selection.data.set();
	else
		while( ptr < endPtr ){
			_s8 difference = _s8(_color(*ptr).getL()) - _s8(brightness);
			if( abs( difference ) <= tolerance )
				selection.data.set( ptr - startPtr );
			ptr++;
		}
	
	return move(selection);
}