#include "_type/type.rect.h"

//////////
// AREA //
//////////

_area::_area( _rect rc ){
	rects.push_back( rc );
}

_area::_area(int){}

void _area::push_back( _rect rc ){
	this->rects.push_back( rc );
}

void _area::push_back( const _area ar ){
	for( const _rect& rc : ar.rects )
		this->rects.push_back( rc );
}

void _area::clear(){
	this->rects.clear();
}

list<_rect>::iterator _area::begin(){
	return this->rects.begin();
}

list<_rect>::iterator _area::end(){
	return this->rects.end();
}

_area _area::operator-( const _rect dim ) const {
	// Temp Rects
	_area r;
	for( const _rect& rc : this->rects )
		r.add( rc - dim );
	return r;
}

_area& _area::operator-=( const _rect dim ){
	// Temp Rects
	list<_rect> tR = this->rects;
	
	// Clear
	this->rects.clear();
	
	for( const _rect& rc : tR )
		this->add( rc - dim );
	
	return *this;
}

void _area::add( _area cR ){
	for( const _rect& rc : cR ){
		this->rects.push_back( rc );
	}
}

void _area::dump() const {
	for( const _rect &rc : this->rects )
		rc.dump();
}

_area _area::toRelative( const _rect absDim ) const {
	_area ar = *this;
	for( _rect &rc : ar.rects )
		rc = rc.toRelative( absDim );
	return ar;
}

_area _area::operator&( const _rect limits ) const {
	_area ar = *this;
	for( _rect &rc : ar.rects )
		rc &= limits;
	return ar;
}

_area& _area::operator&=( const _rect limits ){
	for( _rect &rc : this->rects )
		rc &= limits;
	return *this;
}

bool _area::empty(){
	return this->rects.empty();
}

//////////
// RECT //
//////////
_rect::_rect( _coord x , _coord y , _length width , _length height ){
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

_rect::_rect() : 
	width( 0 ) , height( 0 ) , x( -1 ) , y( -1 )
{ }

_coord _rect::getX2() const { return this->x + _coord(this->width) - 1; }
_coord _rect::getY2() const { return this->y + _coord(this->height) - 1; }
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
	return ( this->x <= x && this->y <= y && ( this->x + _coord(this->width) ) >= x && ( this->y + _coord(this->height) ) >= y )
		? true : false;
}

bool _rect::contains( const _rect rc ) const {
	return ( this->x < rc.x && this->y < rc.y && (_coord(this->height) + this->y) > (rc.height + rc.y) && (_coord(this->width) + this->x) > (rc.width + rc.x) )
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

bool _rect::intersectsWith( const _area other ) const {
	for( const _rect& r : other.rects )
	{
		if( this->x > r.getX2() || this->getX2() < r.x || this->y > r.getY2() || this->getY2() < r.y )
			continue;
		else
			return true;
	}	
	return false;
}

_rect _rect::toRelative( const _rect rc ) const {
	return _rect( this->x - rc.x , this->y - rc.y , this->width , this->height );
}

_rect _rect::operator+( const _margin p ) const {
	return _rect::fromCoords( this->x - p.getLeft() , this->y - p.getTop() , this->getX2() + p.getRight() , this->getY2() + p.getBottom() );
}

_rect _rect::operator-( const _padding p ) const {
	return _rect::fromCoords( this->x + p.getLeft() , this->y + p.getTop() , this->getX2() - p.getRight() , this->getY2() - p.getBottom() );
}

_rect& _rect::operator+=( const _margin p ){
	return (*this = _rect::fromCoords( this->x - p.getLeft() , this->y - p.getTop() , this->getX2() + p.getRight() , this->getY2() + p.getBottom() ) );
}

_rect& _rect::operator-=( const _padding p ){
	return (*this = _rect::fromCoords( this->x + p.getLeft() , this->y + p.getTop() , this->getX2() - p.getRight() , this->getY2() - p.getBottom() ) ); 
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

bool _rect::operator==( const _rect other ) const 
{
	return other.x == this->x && other.y == this->y && other.width == this->width && other.height == this->height;
}