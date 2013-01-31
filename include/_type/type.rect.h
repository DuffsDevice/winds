#ifndef _WIN_T_RECT_
#define _WIN_T_RECT_

#include "_type/type.h"
#include <cmath>
#include <algorithm>

class _rect{

	public:
	
		//! Define Inside because of cicular dependencies...
		class _area
		{
			friend class _rect;
			
			private:
				
				_list<_rect> t_rects;
				
			public:
				
				//! Ctor
				_area( initializer_list<_rect> rects ) : t_rects( rects ) {}
				_area(){}
					
				//! Push-back Aliases
				void add( _rect rc ){ if( rc.isValid() ) t_rects.push_back( rc ); }
				void add( const _area&& cR ){ move( cR.t_rects.begin() , cR.t_rects.end() , back_inserter( t_rects ) ); }
				void add( const _area& cR ){ copy( cR.t_rects.begin() , cR.t_rects.end() , back_inserter( t_rects ) ); }
				
				//! Clear
				void clearRects(){ t_rects.clear(); }
				
				//! Iteration
				_list<_rect>::iterator begin(){ return t_rects.begin(); }
				_list<_rect>::iterator end(){ return t_rects.end(); }
				
				//! dump!
				void dump() const {	for( const _rect &rc : t_rects ) rc.dump(); }
				
				//! Cut the supplied Rectangle off
				_area& reduce( const _rect& dim );
				
				//! Relativate all t_rects
				_area& toRelative( const _coord absX , const _coord absY );
				
				//! Clip all t_rects to the supplied one
				_area& clipToIntersect( const _rect limits );
				
				//! Check for contained t_rects
				bool empty(){ return t_rects.empty(); }
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
		**/
		_rect( _coord x , _coord y , _length width , _length height ) : width( width ) , height( height ) , x( x ) , y( y ) {}
		
		//! All about Setters and Getters...
		_coord getX2() const { return x + _coord(width) - 1; }
		_coord getY2() const { return y + _coord(height) - 1; }
		void setX2( _coord x2 ){ width = x2 - x + 1; }
		void setY2( _coord y2 ){ height = y2 - y + 1; }
		
		/**
		 * Default Constructor
		**/
		_rect() : width( 0 ) , height( 0 ) , x( -1 ) , y( -1 ) {}
		
		void dump() const ;
		
		//! Check for reasonable dimensions
		bool isValid() const { return !( width < 1 || height < 1 ); }
		
		bool contains( const _coord x , const _coord y ) const {
			return !( x < this->x || y < this->y || x > getX2() || y > getY2() );
		}
		
		bool contains( const _rect& rc ) const {
			return !( rc.x < x || rc.y < y || rc.getX2() > getX2() || rc.getY2() > getY2() );
		}
		
		//! Methods to generate a Rect
		static _rect fromCoords( const _coord left , const _coord top , const _coord right , const _coord bottom )
		{
			return _rect( left , top , _length( right - left + 1 ) , _length( bottom - top + 1 ) );
		}
		
		//! Make the Rect Relative to a specific position
		//! absX and absY specify to what the resulting rectangle will be relative
		_rect& toRelative( const _coord absX , const _coord absY )
		{
			this->x -= absX; this->y -= absY; return *this;
		}
		
		//! Make the Rect Relative to a specific position
		//! absX and absY specify to what the resulting rectangle will be relative
		_rect relativeVersion( const _coord absX , const _coord absY ) const
		{
			return _rect( this->x - absX, this->y - absY , this->width , this->height );
		}
		
		//! Returns an _area (=_list of rectangle-pieces) rooting from an Rectangle AND'ed with this one
		_rect& clipToIntersect( const _rect rect )
		{
			return *this = _rect::fromCoords( max( this->x , rect.x ) , max( this->y , rect.y ) , min( this->getX2() , rect.getX2() ) , min( this->getY2() , rect.getY2() ) );
		}
		
		//! Returns an _area (=_list of rectangle-pieces) rooting from an Rectangle OR'd with this one
		_area combine( const _rect& other ) const ;
		
		//! Returns a Rectangle containing both this rect and the passed one
		_rect& expandToInclude( const _rect rect )
		{
			return *this = _rect::fromCoords( min( this->x , rect.x ) , min( this->y , rect.y ) , max( this->getX2() , rect.getX2() ) , max( this->getY2() , rect.getY2() ) );
		}
		
		//! Expand the rect by applying a margin
		_rect& applyMargin( const _margin p )
		{
			return (*this = _rect::fromCoords( this->x - p.left , this->y - p.top , this->getX2() + p.right , this->getY2() + p.bottom ) );
		}
		
		//! Crop the Rect by applying a padding
		_rect& applyPadding( const _padding p )
		{
			return (*this = _rect::fromCoords( this->x + p.left , this->y + p.top , this->getX2() - p.right , this->getY2() - p.bottom ) ); 
		}
		
		//! Cut 'other' off of me
		_area reduce( const _rect& other ) const ;
		
		//! Check if the rect equals another
		bool equals( const _rect other ) const {
			return other.x == this->x && other.y == this->y && other.width == this->width && other.height == this->height;
		}
		
		bool intersectsWith( const _rect& other ) const 
		{
			return ( this->x > other.getX2() || this->getX2() < other.x || this->y > other.getY2() || this->getY2() < other.y ) ? false : true; 
		}
		bool intersectsWith( const _area& other ) const;
		
		bool operator==( const _rect other )
		{
			return !( other.x != this->x || other.y != this->y || other.width != this->width || other.height != this->height );
		}
		
		noinline bool operator!=( const _rect other )
		{
			return other.x != this->x || other.y != this->y || other.width != this->width || other.height != this->height;
		}
};

typedef _rect::_area _area;

#endif