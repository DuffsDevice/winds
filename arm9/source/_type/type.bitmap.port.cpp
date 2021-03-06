#include <_type/type.bitmap.port.h>

void _bitmapPort::drawPixel( _coord x , _coord y , _color color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawPixel( x , y , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::fill( _color color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.fill( color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawVerticalLine( _coord x , _coord y , _length length , _color color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawVerticalLine( x , y , length , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawHorizontalLine( _coord x , _coord y , _length length , _color color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawHorizontalLine( x , y , length , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawVerticalDottedLine( _coord x , _coord y , _length length , _color color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawVerticalDottedLine( x , y , length , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawHorizontalDottedLine( _coord x , _coord y , _length length , _color color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawHorizontalDottedLine( x , y , length , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawLine( _coord x1 , _coord y1 , _coord x2 , _coord y2 , _color color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawLine( x1 , y1 , x2 , y2 , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawRect( _coord x , _coord y , _length w , _length h , _color color )
{			
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawRect( x , y , w , h , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawDottedRect( _coord x , _coord y , _length w , _length h , _color color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawDottedRect( x , y , w , h , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawFilledRect( _coord x , _coord y , _length w , _length h , _color color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawFilledRect( x , y , w , h , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawVerticalGradient( _coord x , _coord y , _length w , _length h , _color fromColor , _color toColor )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawVerticalGradient( x , y , w , h , fromColor , toColor );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawHorizontalGradient( _coord x , _coord y , _length w , _length h , _color fromColor , _color toColor )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawHorizontalGradient( x , y , w , h , fromColor , toColor );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawCircle( _coord xc, _coord yc, _length radius, _color color )
{			
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawCircle( xc , yc , radius , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawFilledCircle( _coord xc, _coord yc, _length radius, _color color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawFilledCircle( xc , yc , radius , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawEllipse( _coord xc, _coord yc, _length a, _length b, _color color)
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawEllipse( xc , yc , a , b , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawFilledEllipse( _coord xc, _coord yc, _length a, _length b, _color color)
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawFilledEllipse( xc , yc , a , b , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawString( _coord x0 , _coord y0 , _fontHandle font , _literal str , _color color , _fontSize fontSize )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawString( x0 , y0 , font , str , color , fontSize );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawString( _coord x0 , _coord y0 , _fontHandle font , const wstring& str , _color color , _fontSize fontSize )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawString( x0 , y0 , font , str , color , fontSize );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawString( _coord x0 , _coord y0 , _fontHandle font , _wliteral str , _color color , _fontSize fontSize )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawString( x0 , y0 , font , str , color , fontSize );
		//! Standard Bitmap Routine
	}
}

_length _bitmapPort::drawChar( _coord x0 , _coord y0 , _fontHandle font , _wchar ch , _color color , _fontSize fontSize )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.drawChar( x0 , y0 , font , ch , color , fontSize );
		//! Standard Bitmap Routine
	}
	return font->getCharacterWidth( ch );
}

void _bitmapPort::copy( _coord x , _coord y , _constBitmap& data )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.copy( x , y , data );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::copyTransparent( _coord x , _coord y , _constBitmap& data )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.copyTransparent( x , y , data );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::copyHorizontalStretch( _coord x , _coord y , _length w , _constBitmap& data )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.copyHorizontalStretch( x , y , w , data );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::copyVerticalStretch( _coord x , _coord y , _length h , _constBitmap& data )
{
	for( const _rect& rc : clippingRects )
	{
		this->base.setClippingRectUnsafe( rc );
		
		//! Standard Bitmap Routine
		this->base.copyVerticalStretch( x , y , h , data );
		//! Standard Bitmap Routine
	}
}