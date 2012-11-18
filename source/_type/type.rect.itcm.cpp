#include "_type/type.rect.h"

bool _rect::intersectsWith( const _area& other ) const {
	for( const _rect& r : other.t_rects )
	{
		if( this->x > r.getX2() || this->getX2() < r.x || this->y > r.getY2() || this->getY2() < r.y )
			continue;
		else
			return true;
	}	
	return false;
}

void _rect::dump() const {
	//printf("_rect:%d,%d,%d,%d\n",this->x,this->y,this->width,this->height);
}

_area _rect::reduce( const _rect& r2 ) const 
{
	_area out;
	
	if( !this->intersectsWith( r2 ) )
	{
		out.add( *this );
		return out;
	}
	
	_coord this_x2 = this->getX2();
	_coord this_y2 = this->getY2();
	_coord r2_x2 = r2.getX2();
	_coord r2_y2 = r2.getY2();
	
	// Left Top Right Bottom
	bool overlapping[4] = { r2.x > this->x , r2.y > this->y , r2_x2 < this_x2 , r2_y2 < this_y2 };
	
	if( overlapping[0] )
		out.add( _rect::fromCoords( this->x , overlapping[1] ? r2.y : this->y , r2.x - 1 , overlapping[3] ? r2_y2 : this_y2 ) );
	if( overlapping[1] )
		out.add( _rect( this->x , this->y , this->width , r2.y - this->y ) );
	if( overlapping[2] )
		out.add( _rect::fromCoords( r2_x2 + 1 , overlapping[1] ? r2.y : this->y , this_x2 , overlapping[3] ? r2_y2 : this_y2 ) );
	if( overlapping[3] )
		out.add( _rect( this->x , r2_y2 + 1 , this->width , this_y2 - r2_y2 ) );
	
	return out;
}

_area _rect::combine( const _rect& r2 ) const 
{
	_area out;
	
	if( !this->intersectsWith( r2 ) )
	{
		out.add( r2 );
		out.add( *this );
		return out;
	}
	
	_coord this_x2 = this->getX2();
	_coord this_y2 = this->getY2();
	_coord r2_x2 = r2.getX2();
	_coord r2_y2 = r2.getY2();
	
	// Left Top Right Bottom
	bool overlapping[4] = { r2.x < this->x , r2.y < this->y , r2_x2 > this_x2 , r2_y2 > this_y2 };
	_u32 cnt = overlapping[0] + overlapping[1] + overlapping[2] + overlapping[3];
	
	switch( cnt )
	{
		case 0:
			out.add( *this );
			break;
		case 1:		
			out.add( *this );
			if( overlapping[0] )
				out.add( _rect( r2.x , r2.y , this->x - r2.x , r2.height ) );
			else if( overlapping[1] )
				out.add( _rect( r2.x , r2.y , r2.width , this->y - r2.y ) );
			else if( overlapping[2] )
				out.add( _rect( this_x2 + 1 , r2.y , r2_x2 - this_x2 , r2.height ) );
			else if( overlapping[3] )
				out.add( _rect( r2.x , this_y2 + 1 , r2.width , r2_y2 - this_y2 ) );
			break;
		case 2:
			out.add( *this );
			if( overlapping[0] )
				out.add( _rect::fromCoords( r2.x , max( this->y , r2.y ) , this->x - 1 , min( r2_y2 , this_y2 ) ) );
			if( overlapping[1] )
				out.add( _rect( r2.x , r2.y , r2.width , this->y - r2.y ) );
			if( overlapping[2] )
				out.add( _rect::fromCoords( this_x2 + 1 , max( this->y , r2.y ) , r2_x2 , min( r2_y2 , this_y2 ) ) );
			if( overlapping[3] )
				out.add( _rect( r2.x , this_y2 + 1 , r2.width , r2_y2 - this_y2 ) );
			break;
		case 3:
			out.add( r2 );
			if( !overlapping[0] )
				out.add( _rect( this->x , this->y , r2.x - this->x , this->height ) );
			else if( !overlapping[1] )
				out.add( _rect( this->x , this->y , this->width , r2.y - this->y ) );
			else if( !overlapping[2] )
				out.add( _rect( r2_x2 + 1 , this->y , this_x2 - r2_x2 , this->height ) );
			else if( !overlapping[3] )
				out.add( _rect( this->x , r2_y2 + 1 , this->width , this_y2 - r2_y2 ) );
			break;
		case 4:
			out.add( r2 );
			break;
	}
	
	return out;
}