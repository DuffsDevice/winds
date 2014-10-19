#ifndef _WIN_G_DIVIDER_
#define _WIN_G_DIVIDER_

#include <_type/type.gadget.h>
#include <_type/type.gadget.helpers.h>

class _dividerGadget : public _gadget
{
	public:
		
		_u8			margin;
		_dimension	dim;
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn updateHandler( _event );
		
	public:
		
		//! Set Dimension
		void setDimension( _dimension dim );
		
		//! Get dimension
		_dimension getDimension() const { return this->dim; }
		
		//! Get Position
		_coord getPosition() const {
			if( this->dim == _dimension::vertical )
				return this->getX();
			return this->getY();
		}
		
		//! Set Position
		void setPosition( _optValue<_coord> pos , bool ignoreAuto = false ){
			if( this->dim == _dimension::vertical )
				this->setX( pos , ignoreAuto );
			else
				this->setY( pos , ignoreAuto );
		}
		
		//! Get Margin
		_u8 getMargin() const { return this->margin; }
		
		//! Set Margin
		void setMargin( _u8 margin ){
			if( this->margin == margin )
				return;
			this->margin = margin;
			this->update();
		}
		
		//! Ctor
		_dividerGadget( _dimension dim , _optValue<_coord> position , _u8 margin = 4 , _style&& style = _style() );
};

#endif