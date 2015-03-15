#ifndef _WIN_T_BITFIELD_
#define _WIN_T_BITFIELD_

#include <_type/type.h>
#include <algorithm>
//#include <tr2/dynamic_bitset>

//using _bitField = std::tr2::dynamic_bitset<bool>;

class _bitField : public _vector<bool>
{
	public:
		
		//! Ctor
		_bitField( int size , bool allSelected = false ) :
			_vector<bool>( size , allSelected )
		{}
		
		//! Set a bit
		void set( _u32 n , bool value = true ){
			(*this)[n] = true;
		}
		
		//! Set all bits to true
		void set(){
			assign( size() , true );
		}
};

#endif