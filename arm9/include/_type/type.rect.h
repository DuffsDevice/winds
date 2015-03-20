#ifndef _WIN_T_RECT_
#define _WIN_T_RECT_

#include <_type/type.h>
#include <_type/type.border.h>
#include <cmath>
#include <algorithm>

class _rect{

	public:
	
		//! Define Inside because of cicular dependencies...
		class _area
		{
			friend class _rect;
			
			private:
				
				_vector<_rect> rects;
				
			public:
				
				//! Ctors
				_area( _initializerList<_rect> rects ) : rects( move(rects) ) {}
				_area( _vector<_rect> rects ) : rects( move(rects) ) {}
				_area( _rect rc ) : rects( rc.isValid() ? 1 : 0 ) { if( rc.isValid() ) rects.push_back( rc ); }
				_area(){}
				
				//! Copy & Move Ctors
				_area( _area&& ar ) : rects( move(ar.rects) ) {}
				_area( const _area& ar ) : rects( ar.rects ) {}
				
				//! Push-back Aliases
				void add( _rect rc ){
					if( rc.isValid() )
						rects.emplace_back( move(rc) );
				}
				void add( const _area& cR ){
					rects.reserve( rects.size() + cR.rects.size() );
					for( _rect rc : cR.rects )
						add( move(rc) );
				}
				
				//! Clear
				void clearRects(){ rects.clear(); }
				
				//! Iteration
				_vector<_rect>::iterator begin(){ return rects.begin(); }
				_vector<_rect>::iterator end(){ return rects.end(); }
				
				//! Cut the supplied Rectangle off
				_area& reduce( const _rect& dim );
				
				//! Relativate all rects
				_area& toRelative( const _coord absX , const _coord absY );
				_area toRelative( const _coord absX , const _coord absY ) const ;
				_area& toRelative( const _pos position ){ return this->toRelative( position.first , position.second ); }
				_area toRelative( const _pos position ) const { return this->toRelative( position.first , position.second ); }
				
				//! Clip all rects to the supplied one
				_area& clipToIntersect( const _rect& limits );
				
				//! Check for contained rects
				bool isEmpty() const { return rects.empty(); }
		};
		
		//! Dimensions
		_length width;
		_length height;
		
		//! Position
		_coord x;
		_coord y;
	
	public:
		
		/**
		 * Constructor
		 * @param x X-Position
		 * @param y Y-Position
		 * @param width Width of the Rect
		 * @param height Height of the Rect
		 */
		_rect( _coord x , _coord y , _length width , _length height ) : width( width ) , height( height ) , x( x ) , y( y ) {}
		
		/**
		 * Constructor
		 * @param pos (X|Y)-Position
		 * @param size (width|height)
		 */
		_rect( _pos pos , _size size ) : width( size.first ) , height( size.second ) , x( pos.first ) , y( pos.second ) {}
		
		//! All about Setters and Getters...
		_coord getX2() const { return x + _coord(width) - 1; }
		_coord getY2() const { return y + _coord(height) - 1; }
		void setX2( _coord x2 ){ width = x2 - x + 1; }
		void setY2( _coord y2 ){ height = y2 - y + 1; }
		
		/**
		 * Default Constructor
		 */
		_rect() : width( 0 ) , height( 0 ) , x( -1 ) , y( -1 ) {}
		
		//! Check for reasonable dimensions
		bool isValid() const { return width > 0 && height > 0 ; }
		
		bool contains( const _pos position ) const { return this->contains( position.first , position.second ); }
		bool contains( const _coord x , const _coord y ) const {
			return !( x < this->x || y < this->y || x > getX2() || y > getY2() );
		}
		
		bool contains( const _rect& rc ) const {
			return rc.isValid() && !( rc.x < x || rc.y < y || rc.getX2() > getX2() || rc.getY2() > getY2() );
		}
		
		//! Methods to generate a Rect
		static _rect fromCoords( _coord left , _coord top , _coord right , _coord bottom )
		{
			if( left > right )
				std::swap( left , right );
			if( top > bottom )
				std::swap( top , bottom );
			return _rect( left , top , _length( right - left + 1 ) , _length( bottom - top + 1 ) );
		}
		
		//! Make the Rect Relative to a specific position
		//! absX and absY specify to what the resulting rectangle will be relative
		_rect& toRelative( const _pos position ){ return this->toRelative( position.first , position.second ); }
		_rect toRelative( const _pos position ) const { return this->toRelative( position.first , position.second ); }
		_rect& toRelative( const _coord absX , const _coord absY ){
			this->x -= absX; this->y -= absY; return *this;
		}
		_rect toRelative( const _coord absX , const _coord absY ) const {
			return _rect( this->x - absX, this->y - absY , this->width , this->height );
		}
		
		//! Returns an _area (=_list of rectangle-pieces) rooting from an Rectangle AND'ed with this one
		_rect& clipToIntersect( const _rect& rect );
		
		//! Returns an _area (=_list of rectangle-pieces) rooting from an Rectangle OR'd with this one
		_area combine( const _rect& other ) const ;
		
		//! Returns a Rectangle containing both this rect and the passed one
		_rect& expandToInclude( const _rect& rect );
		
		//! Expand the rect by applying a margin
		_rect& applyMargin( const _margin p )
		{
			this->x -= p.left;
			this->y -= p.top;
			this->width = max<_length>( this->width + p.right + p.left , 0 );
			this->height = max<_length>( this->height + p.bottom + p.top , 0 );
			return *this;
		}
		
		//! Crop the Rect by applying a padding
		_rect& applyPadding( const _padding p )
		{
			this->x += p.left;
			this->y += p.top;
			this->width = max<_length>( this->width - p.right - p.left , 0 );
			this->height = max<_length>( this->height - p.bottom - p.top , 0 );
			return *this;
		}
		
		//! Cut 'other' off of me
		_area reduce( const _rect& other ) const ;
		
		//! Check whether 'other' itersects this rect
		bool intersectsWith( const _rect& other ) const {
			return this->x < other.getX2() && this->getX2() > other.x && this->y < other.getY2() && this->getY2() > other.y; 
		}
		//! Check whether any of 'other's rects intersects me
		bool intersectsWith( const _area& other ) const;
		
		//! Check if the rect equals another
		bool operator==( const _rect other ){
			return other.x == this->x && other.y == this->y && other.width == this->width && other.height == this->height;
		}
		
		//! ..or if it doesn't
		bool operator!=( const _rect other ){
			return !(*this == other);
		}
		
		
		// Only used in type.gadget.h
		inline _rect& expand( _length dX , _length dY ){
			this->expandX( dX );
			this->expandY( dY );
			return *this;
		}
		
		noinline _rect& expandX( _length dX )
		{
			if( dX < 0 ){
				this->x += dX;
				this->width -= dX;
			}else if( dX > 0 )
				this->width += dX;
			return *this;
		}
		
		noinline _rect& expandY( _length dY )
		{
			if( dY < 0 ){
				this->y += dY;
				this->height -= dY;
			}else if( dY > 0 )
				this->height += dY;
			return *this;
		}
		
		inline _rect& shrink( _length dX , _length dY ){
			this->shrinkX( dX );
			this->shrinkY( dY );
			return *this;
		}
		
		noinline _rect& shrinkX( _length dX )
		{
			if( dX > 0 ){
				dX = min<_length>( dX , this->width );
				this->x += dX;
				this->width -= dX;
			}else if( dX < 0 ){
				dX = max<_length>( dX , -this->width );
				this->width += dX;
			}
			return *this;
		}
		
		noinline _rect& shrinkY( _length dY )
		{
			if( dY > 0 ){
				dY = min<_length>( dY , this->height );
				this->y += dY;
				this->height -= dY;
			}else if( dY < 0 ){
				dY = max<_length>( dY , -this->height );
				this->height += dY;
			}
			return *this;
		}
};

typedef _rect::_area _area;

#endif