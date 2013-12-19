#ifndef _WIN_T_GRID_
#define _WIN_T_GRID_

#include "_type/type.h"
#include "_type/type.gadget.h"

class _grid
{
	private:
	
		_length spaceX;
		_length spaceY;
		
		_length offsetX;
		_length offsetY;
		
		_vector<_vector<_gadget*>>	gadgets;
	
	public:
		
		//! Full Ctor
		_grid( _gadget* parent , _length spaceX , _length spaceY , _length offsetX , _length offsetY );
		
		//! Basic Ctor
		_grid( _gadget* parent , _length spaceX , _length spaceY ) :
			_grid( parent , spaceX , spaceY , spaceX , spaceY )
		{}
		
		void setField( _u32 col , _u32 col , _gadget* gadget )
		{
			
		}
}

#endif