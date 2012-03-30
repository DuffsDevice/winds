#include "_type/type.rect.h"

_rect::_rect( _coord x , _coord y , _length width , _length height ){
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

_rect::_rect() : 
	width( 1 ) , height( 1 ) , x( -1 ) , y( -1 )
{ }

_coord _rect::getX2() const { return this->x + this->width - 1; }
_coord _rect::getY2() const { return this->y + this->height - 1; }
void _rect::setX2( _coord x2 ){ this->width = x2 - this->x + 1; }
void _rect::setY2( _coord y2 ){ this->height = y2 - this->y + 1; }
_coord _rect::getX() const { return this->x; }
_coord _rect::getY() const { return this->y; }
void _rect::setX( _coord x ){ this->x = x; }
void _rect::setY( _coord y ){ this->y = y; }
_length _rect::getWidth() const { return this->width; }
_length _rect::getHeight() const { return this->height; }
void _rect::setWidth( _length width ){ this->width = width; }
void _rect::setHeight( _length height ){ this->height = height; }

bool _rect::isValid() const {
	return ( this->width > 0 && this->height > 0 ) ? true : false;
}

bool _rect::contains( const _coord x , const _coord y ) const {
	return ( this->x <= x && this->y <= y && ( this->x + this->width ) >= x && ( this->y + this->height ) >= y )
		? true : false;
}

bool _rect::contains( const _rect rc ) const {
	return ( this->x < rc.x && this->y < rc.y && (this->height + this->y) > (rc.height + rc.y) && (this->width + this->x) > (rc.width + rc.x) )
		? true : false;
}

_rect _rect::fromCoords( const _coord left , const _coord top , const _coord right , const _coord bottom ){
	_rect rc;
	rc.x = left;
	rc.y = top;
	rc.width = right - left + 1;
	rc.height = bottom - top + 1;
	return rc;
}

bool _rect::intersectsWith( const _rect other ) const {
	if( this->x > other.getX2() || this->getX2() < other.x || this->y > other.getY2() || this->getY2() < other.y )
		return false;
	return true;
}

_rect _rect::toRelative( const _rect rc ) const {
	return _rect( this->x - rc.x , this->y - rc.y , this->width , this->height );
}

_rect _rect::operator+( const _margin p ) const {
	return _rect::fromCoords( this->x - p.left , this->y - p.top , this->getX2() + p.right , this->getY2() + p.bottom );
}

_rect _rect::operator-( const _padding p ) const {
	return _rect::fromCoords( this->x + p.left , this->y + p.top , this->getX2() - p.right , this->getY2() - p.bottom );
}

_rect& _rect::operator+=( const _margin p ){
	return (*this = _rect::fromCoords( this->x - p.left , this->y - p.top , this->getX2() + p.right , this->getY2() + p.bottom ) );
}

_rect& _rect::operator-=( const _padding p ){
	return (*this = _rect::fromCoords( this->x + p.left , this->y + p.top , this->getX2() - p.right , this->getY2() - p.bottom ) ); 
}

_rect _rect::operator+( const _rect rc ) const {
	_coord minX = min( this->x , rc.x );
	_coord minY = min( this->y , rc.y );
	_length maxX = max( this->width + this->x , rc.width + rc.x );
	_length maxY = max( this->height + this->y , rc.height + rc.y );
	return _rect( minX , minY , maxX - minX , maxY - minY );
}

void _rect::dump() const {
	printf("_rect:%d,%d,%d,%d\n",this->x,this->y,this->width,this->height);
}

_area _rect::operator-( const _rect other ) const 
{
	if( !this->intersectsWith( other ) )
	{
		_area out;
		out.push_back( *this );
		return out;
	}
	else{
		
		_rect r1 = other;
		_rect r2 = *this;
		
		_area out;
		
		// Horizontal Blocks above and underneath r1
		if( r1.y > r2.y ){
			out.push_back( _rect( r2.x , r2.y , r2.width , r1.y - r2.y ) );
			r2.height -= r1.y - r2.y;
			r2.y = r1.y;
		}
		if( r1.getY2() < r2.getY2() ){
			out.push_back( _rect( r2.x , r1.getY2() + 1 , r2.width , r2.getY2() - r1.getY2() ) );
			r2.setY2( r1.getY2() );
		}
		
		// Vertical Blocks beside r1
		if( r1.x > r2.x )
			out.push_back( _rect( r2.x , r2.y , r1.x - r2.x , r2.height ) );
		if( r1.getX2() < r2.getX2() )
			out.push_back( _rect( r1.getX2() + 1 , r2.y , r2.getX2() - r1.getX2() , r2.height ) );
		
		return out;
	}
	return _area();
}

_area _rect::operator^( const _rect other ) const
{
	if( !this->intersectsWith( other ) )
	{
		_area out;
		out.push_back( *this );
		out.push_back( other );
		return out;
	}
	else{
		
		_rect r1 = this->operator&( other ); // Intersection
		_rect r2 = *this;
		
		_area out;
		
		//! Get Border of this XOR'd with the Intersecting Part
		
			// Horizontal Blocks above and underneath r1
			if( r1.y > r2.y ){
				out.push_back( _rect( r2.x , r2.y , r2.width , r1.y - r2.y ) );
				r2.height += r2.y - r1.y;
				r2.y = r1.y;
			}
			if( r1.getY2() < r2.getY2() ){
				out.push_back( _rect( r2.x , r1.getY2() + 1, r2.width , r2.getY2() - r1.getY2() ) );
				r2.setY2( r1.getY2() );
			}
			
			r2.height = r2.getY2() - r2.y + 1;
			
			// Vertical Blocks beside r1
			if( r1.x > r2.x )
				out.push_back( _rect( r2.x , r2.y , r1.x - r2.x , r2.height ) );
			if( r1.getX2() < r2.getX2() )
				out.push_back( _rect( r1.getX2() + 1 , r2.y , r2.getX2() - r1.getX2() , r2.height ) );
		
		//! Get Border of 'other' XOR'd with the Intersecting Part
			r2 = other;
			
			// Horizontal Blocks above and underneath r1
			if( r1.y > r2.y ){
				out.push_back( _rect( r2.x , r2.y , r2.width , r1.y - r2.y ) );
				r2.height += r2.y - r1.y;
				r2.y = r1.y;
			}
			if( r1.getY2() < r2.getY2() ){
				out.push_back( _rect( r2.x , r1.getY2() + 1, r2.width , r2.getY2() - r1.getY2() ) );
				r2.setY2( r1.getY2() );
			}
			
			r2.height = r2.getY2() - r2.y + 1;
			
			// Vertical Blocks beside r1
			if( r1.x > r2.x )
				out.push_back( _rect( r2.x , r2.y , r1.x - r2.x , r2.height ) );
			if( r1.getX2() < r2.getX2() )
				out.push_back( _rect( r1.getX2() + 1 , r2.y , r2.getX2() - r1.getX2() , r2.height ) );
		
		return out;
	}
	return _area();
}

_area _rect::operator|( const _rect other ) const 
{
	if( !this->intersectsWith( other ) )
	{
		_area out;
		out.push_back( *this );
		out.push_back( other );
		return out;
	}
	else{
		
		_rect r1 = *this;
		_rect r2 = other;
		
		_area out;
		
		// Horizontal Blocks above and underneath r1
		if( r1.y > r2.y ){
			out.push_back( _rect( r2.x , r2.y , r2.width , r1.y - r2.y ) );
			r2.height += r2.y - r1.y;
			r2.y = r1.y;
		}
		if( r1.getY2() < r2.getY2() ){
			out.push_back( _rect( r2.x , r1.getY2() + 1, r2.width , r2.getY2() - r1.getY2() ) );
			r2.setY2( r1.getY2() );
		}
		
		r2.height = r2.getY2() - r2.y + 1;
		
		// Vertical Blocks beside r1
		if( r1.x > r2.x )
			out.push_back( _rect( r2.x , r2.y , r1.x - r2.x , r2.height ) );
		if( r1.getX2() < r2.getX2() )
			out.push_back( _rect( r1.getX2() + 1 , r2.y , r2.getX2() - r1.getX2() , r2.height ) );
		
		out.push_back( *this );
		
		return out;
	}
	return _area();
}

_rect _rect::operator&( const _rect rect ) const 
{
	_coord x1 = max( this->x , rect.x );
	_coord y1 = max( this->y , rect.y );

	_coord x2 = min( this->getX2() , rect.getX2() );
	_coord y2 = min( this->getY2() , rect.getY2() );

	return _rect( x1 , y1 , x2 - x1 + 1 , y2 - y1 + 1 );
}