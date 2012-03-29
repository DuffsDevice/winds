#include "_type/type.bitmapPort.h"

_bitmapPort::_bitmapPort( _bitmap* bm ) : base( bm ){}

void _bitmapPort::addClippingRects( _rect cR ){
	clippingRects.push_back( cR );
}

void _bitmapPort::addClippingRects( _area cR ){
	for( vector<_rect>::iterator i = cR.begin() ; i < cR.end() ;i++ ){
		clippingRects.push_back( *i );
	}
}

void _bitmapPort::resetClippingRects(){
	clippingRects.erase ( clippingRects.begin() , clippingRects.end() );
	clippingRects.push_back( _rect( 0 , 0 , this->base->getWidth() , this->base->getHeight() ) );
}

void _bitmapPort::deleteClippingRects(){
	clippingRects.erase ( clippingRects.begin() , clippingRects.end() );
}

_pixel& _bitmapPort::operator()( const _coord x , const _coord y ){
	return (*this->base)( x , y );
}

_pixel& _bitmapPort::operator[]( const _u32 n ){
	return (*this->base)[n];
}

_length _bitmapPort::getWidth() const {
	return this->base->getWidth();
}

_length _bitmapPort::getHeight() const {
	return this->base->getHeight();
}

_pixel _bitmapPort::getPixel( const _coord x , const _coord y ) const 
{
	return this->base->getPixel( x , y );
}

void _bitmapPort::drawPixel( _coord x , _coord y , _pixel color )
{
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->drawPixel( x , y , color );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

void _bitmapPort::fill( _pixel color )
{
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->fill( color );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

void _bitmapPort::drawVerticalLine( _coord x , _coord y , _length length , _pixel color )
{
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->drawVerticalLine( x , y , length , color );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

void _bitmapPort::drawHorizontalLine( _coord x , _coord y , _length length , _pixel color )
{
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->drawHorizontalLine( x , y  , length , color );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

void _bitmapPort::drawRect( _coord x , _coord y , _length w , _length h , _pixel color )
{			
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->drawRect( x , y , w , h , color );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

void _bitmapPort::drawFilledRect( _coord x , _coord y , _length w , _length h , _pixel color )
{
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->drawFilledRect( x , y , w , h , color );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

void _bitmapPort::drawVerticalGradient( _coord x , _coord y , _length w , _length h , _pixel fromColor , _pixel toColor )
{
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->drawVerticalGradient( x , y , w , h , fromColor , toColor );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

void _bitmapPort::drawHorizontalGradient( _coord x , _coord y , _length w , _length h , _pixel fromColor , _pixel toColor )
{
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->drawHorizontalGradient( x , y , w , h , fromColor , toColor );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

void _bitmapPort::drawCircle( _coord xc, _coord yc, _length radius, _pixel color )
{			
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->drawCircle( xc , yc , radius , color );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

void _bitmapPort::drawFilledCircle( _coord xc, _coord yc, _length radius, _pixel color )
{
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->drawFilledCircle( xc , yc , radius , color );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

void _bitmapPort::drawEllipse( _coord xc, _coord yc, _length a, _length b, _pixel color)
{
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->drawEllipse( xc , yc , a , b , color );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

void _bitmapPort::drawFilledEllipse( _coord xc, _coord yc, _length a, _length b, _pixel color)
{
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->drawFilledEllipse( xc , yc , a , b , color );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

void _bitmapPort::drawString( _coord x0 , _coord y0 , _font* font , string str , _pixel color)
{
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->drawString( x0 , y0 , font , str , color );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

_length _bitmapPort::drawChar( _coord x0 , _coord y0 , _font* font , _char ch , _pixel color )
{
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->drawChar( x0 , y0 , font , ch , color );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
	
	return font->getCharacterWidth( ch );
}

void _bitmapPort::copy( _coord x , _coord y , const _bitmap* data )
{
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->copy( x , y , data );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

void _bitmapPort::copy( _coord x , _coord y , _bitmapPort data )
{
	this->copy( x , y , data.base );
}

void _bitmapPort::copyTransparent( _coord x , _coord y , const _bitmap* data , _pixel transparentColor )
{
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->copyTransparent( x , y , data , transparentColor );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

void _bitmapPort::copyTransparent( _coord x , _coord y , _bitmapPort data , _pixel transparentColor )
{
	this->copyTransparent( x , y , data.base , transparentColor );
}

void _bitmapPort::copyHorizontalStretch( _coord x , _coord y , _length w , const _bitmap* data )
{
	_rect tempRect = this->base->getClippingRect();
	
	for( deque< _rect >::iterator it = clippingRects.begin() ; it < clippingRects.end() ; it++ )
	{
		this->base->setClippingRect( *it );
		
		//! Standard Bitmap Routine
		this->base->copyHorizontalStretch( x , y , w , data );
		//! Standard Bitmap Routine
	}
	
	this->base->setClippingRect( tempRect );
}

void _bitmapPort::copyHorizontalStretch( _coord x , _coord y , _length w , _bitmapPort data )
{
	this->copyHorizontalStretch( x , y , w , data.base );
}