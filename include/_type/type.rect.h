#ifndef _WIN_T_RECT_
#define _WIN_T_RECT_

#include "type.h"
#include <cmath>
#include <list>
#include <algorithm>

class _rect{

	public:
		
		//! Dimensions
		_length width;
		_length height;
		
		//! Position
		_coord x;
		_coord y;
		
		//! Define Inside because of cicular dependencies...
		class _area
		{
			friend class _rect;
			
			private:
				
				list<_rect> rects;
				
			public:
				
				_area( _rect rc );
				
				_area( int = 0 );
				
				void push_back( _rect rc );
				
				void push_back( const _area ar );
				
				void clear();
				
				list<_rect>::iterator begin();
				
				list<_rect>::iterator end();
				
				_area operator-( const _rect dim ) const ;
				
				_area& operator-=( const _rect dim );
				
				void add( _area cR );
				
				void dump() const ;
				
				_area toRelative( const _rect absDim ) const ;
				
				_area operator&( const _rect limits ) const ;
				
				_area& operator&=( const _rect limits );
				
				bool empty();
		};
	
	public:
		
		/**
		 * Constructor
		 * @param x X-Position
		 * @param y Y-Position
		 * @param width Width of the Rect
		 * @param height Height of the Rect
		**/
		_rect( _coord x , _coord y , _length width , _length height );
		
		//! All about Setters and Getters...
		_coord getX2() const ;		
		_coord getY2() const ;		
		_coord getX() const ;		
		_coord getY() const ;				
		_length getWidth() const ;		
		_length getHeight() const ;
		void setX( _coord x );		
		void setY( _coord y );
		void setX2( _coord x2 );		
		void setY2( _coord y2 );	
		void setWidth( _length width );		
		void setHeight( _length heigth );
		
		/**
		 * Default Constructor
		**/
		_rect();
		
		void dump() const ;
		
		bool isValid() const ;
		
		bool contains( const _coord x , const _coord y ) const ;
		
		bool contains( const _rect rc ) const ;
		
		//! Methods to generate a Rect
		static _rect fromCoords( const _coord left , const _coord top , const _coord right , const _coord bottom );
		
		//! Make the Rect Relative to a specific position
		_rect toRelative( const _rect absolute ) const;
		
		//! Returns an _area (=list of rectangle-pieces) rooting from an Rectangle XOR'd with this one
		_area operator^( const _rect other ) const ;
		
		//! Returns an _area (=list of rectangle-pieces) rooting from an Rectangle AND'ed with this one
		_rect operator&( const _rect other ) const ;
		_rect& operator&=( const _rect other ){ return *this = *this & other; }
		
		//! Returns an _area (=list of rectangle-pieces) rooting from an Rectangle OR'd with this one
		_area operator|( const _rect other ) const ;
		
		//! Returns a Rectangle containing both this rect and the passed one
		_rect operator+( const _rect other ) const ;
		_rect& operator+=( const _rect other ){ return *this = *this + other; }
		
		//! Expand the rect by applying a margin
		_rect operator+( const _margin p ) const ;
		_rect& operator+=( const _margin p );
		
		//! Cut 'other' off of me
		_area operator-( const _rect other ) const ;
		
		//! Crop the Rect by applying a padding
		_rect operator-( const _padding p ) const ;
		_rect& operator-=( const _padding p );
		
		//! Check if the rect equals another
		bool operator==( const _rect other ) const ;
		
		bool intersectsWith( const _rect other ) const ;
		bool intersectsWith( const _area other ) const;
};

typedef _rect::_area _area;

#endif