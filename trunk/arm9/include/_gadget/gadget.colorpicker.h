#ifndef _WIN_G_COLORPICKER_
#define _WIN_G_COLORPICKER_

#include "_type/type.gadget.h"
#include "_gadget/gadget.imagegadget.h"

class _colorpicker : public _gadget
{
	protected:
		
		//! Currently adjusted components
		_u8		lum;
		_u8		sat;
		_u16	hue;
		
		//! Imagegadgets for both hue-sat-table
		//! And brightness-table
		_gadget*	hueSatTable;
		_gadget*	lumTable;
		_bitmap		hueSatImage;
		
		_callbackReturn lumRefreshHandler( _event );
		_callbackReturn hueSatRefreshHandler( _event );
		_callbackReturn refreshHandler( _event );
		_callbackReturn resizeHandler( _event );
		_callbackReturn inputHandler( _event );
		
		// Refreshes the big colorful base gradient
		void refreshBigGradient();
		
	public:
		
		//! Select a date
		void setColor( _pixel color );
		void setIntValue( _pixel color ){ this->setColor( color ); }
		
		//! Get the selected date
		_pixel getColor() const ;
		_pixel getIntValue() const { return this->getColor(); }
		
		//! Ctor
		_colorpicker( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _pixel initialColor = COLOR_BLUE , _style&& style = _style() );
		
		//! Dtor
		~_colorpicker(){
			this->removeChildren( true );
		}
};

#endif