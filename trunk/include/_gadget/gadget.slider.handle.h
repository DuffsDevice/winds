#ifndef _WIN_G_SLIDERHANDLE_
#define _WIN_G_SLIDERHANDLE_

#include "_type/type.h"
#include "_type/type.gadget.h"

class _sliderHandle : public _gadget{
	
	protected:
		
		_dimension dimension;
		
		static _callbackReturn refreshHandler( _event event );
		
	public:
		
		void setDimension( _dimension dim )
		{
			this->dimension = dim;
			if( dim == _dimension::horizontal )
				this->setSize( 7 , 13 );
			else
				this->setSize( 14 , 7 );
		}
		
		//! Ctors
		_sliderHandle( _coord x , _coord y , _dimension dimension , _style&& style = _style() );
};
#endif