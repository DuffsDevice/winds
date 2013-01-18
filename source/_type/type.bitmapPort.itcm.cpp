#include "_type/type.bitmapPort.h"

void _bitmapPort::drawPixel( _coord x , _coord y , _pixel color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->drawPixel( x , y , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::fill( _pixel color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->fill( color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::replaceColor( _pixel color , _pixel replace )
{
	//! Standard Bitmap Routine
	this->base->replaceColor( color , replace);
}

void _bitmapPort::drawVerticalLine( _coord x , _coord y , _length length , _pixel color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->drawVerticalLine( x , y , length , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawHorizontalLine( _coord x , _coord y , _length length , _pixel color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->drawHorizontalLine( x , y , length , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawLine( _coord x1 , _coord y1 , _coord x2 , _coord y2 , _pixel color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->drawLine( x1 , y1 , x2 , y2 , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawRect( _coord x , _coord y , _length w , _length h , _pixel color )
{			
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->drawRect( x , y , w , h , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawFilledRect( _coord x , _coord y , _length w , _length h , _pixel color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->drawFilledRect( x , y , w , h , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawVerticalGradient( _coord x , _coord y , _length w , _length h , _pixel fromColor , _pixel toColor )
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->drawVerticalGradient( x , y , w , h , fromColor , toColor );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawHorizontalGradient( _coord x , _coord y , _length w , _length h , _pixel fromColor , _pixel toColor )
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->drawHorizontalGradient( x , y , w , h , fromColor , toColor );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawCircle( _coord xc, _coord yc, _length radius, _pixel color )
{			
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->drawCircle( xc , yc , radius , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawFilledCircle( _coord xc, _coord yc, _length radius, _pixel color )
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->drawFilledCircle( xc , yc , radius , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawEllipse( _coord xc, _coord yc, _length a, _length b, _pixel color)
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->drawEllipse( xc , yc , a , b , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawFilledEllipse( _coord xc, _coord yc, _length a, _length b, _pixel color)
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->drawFilledEllipse( xc , yc , a , b , color );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::drawString( _coord x0 , _coord y0 , _font* font , string str , _pixel color , _u8 fontSize )
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->drawString( x0 , y0 , font , str , color , fontSize );
		//! Standard Bitmap Routine
	}
}

_length _bitmapPort::drawChar( _coord x0 , _coord y0 , _font* font , _char ch , _pixel color , _u8 fontSize )
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->drawChar( x0 , y0 , font , ch , color , fontSize );
		//! Standard Bitmap Routine
	}
	return font->getCharacterWidth( ch );
}

void _bitmapPort::copy( _coord x , _coord y , const _bitmap& data )
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->copy( x , y , data );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::copyTransparent( _coord x , _coord y , const _bitmap& data )
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->copyTransparent( x , y , data );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::copyHorizontalStretch( _coord x , _coord y , _length w , const _bitmap& data )
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->copyHorizontalStretch( x , y , w , data );
		//! Standard Bitmap Routine
	}
}

void _bitmapPort::copyVerticalStretch( _coord x , _coord y , _length h , const _bitmap& data )
{
	for( const _rect& rc : clippingRects )
	{
		this->base->setClippingRect( rc );
		
		//! Standard Bitmap Routine
		this->base->copyVerticalStretch( x , y , h , data );
		//! Standard Bitmap Routine
	}
}