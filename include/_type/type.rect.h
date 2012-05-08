#ifndef _WIN_T_RECT_
#define _WIN_T_RECT_

#include "type.h"
#include <cmath>
#include <vector>
#include <stdio.h>
#include <algorithm>

class _rect{

	public:
		
		//! Dimensions
		_length width;
		_length height;
		
		//! Position
		_coord x;
		_coord y;
		
		class _area
		{
			private:
				
				vector<_rect> rects;
				
			public:
				
				_area( _rect rc ){
					rects.push_back( rc );
				}
				
				_area( int = 0 ){}
				
				void push_back( _rect rc ){
					this->rects.push_back( rc );
				}
				
				void push_back( _area ar ){
					for( _rect& rc : ar.rects )
						this->rects.push_back( rc );
				}
				
				void clear(){
					this->rects.clear();
				}
				
				_rect& operator[](int u){
					return this->rects[u];
				}
				
				vector<_rect>::iterator begin(){
					return this->rects.begin();
				}
				
				vector<_rect>::iterator end(){
					return this->rects.end();
				}
				
				_area operator-( const _rect dim ) const {
					// Temp Rects
					_area r;
					for( auto it = this->rects.begin(); it < this->rects.end() ; it++ ){
						r.add( *it - dim );
					}
					return r;
				}
				
				_area& operator-=( const _rect dim ){
					// Temp Rects
					vector<_rect> tR = this->rects;
					this->rects.clear();
					for( auto it = tR.begin(); it < tR.end() ; it++ ){
						this->add( *it - dim );
					}
					return *this;
				}
				
				void add( _area cR ){
					for( vector<_rect>::iterator i = cR.begin() ; i < cR.end() ;i++ ){
						this->rects.push_back( *i );
					}
				}
				
				void dump() const {
					for( const _rect &rc : this->rects )
						rc.dump();
				}
				
				_area toRelative( const _rect absDim ) const {
					_area ar = *this;
					for( _rect &rc : ar.rects )
						rc = rc.toRelative( absDim );
					return ar;
				}
				
				_area operator&( const _rect limits ) const {
					_area ar = *this;
					for( _rect &rc : ar.rects )
						rc &= limits;
					return ar;
				}
				
				_area& operator&=( const _rect limits ){
					for( _rect &rc : this->rects )
						rc &= limits;
					return *this;
				}
				
				bool empty(){
					return this->rects.empty();
				}
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
		
		bool intersectsWith( _rect other ) const ;
};

typedef _rect::_area _area;

#endif