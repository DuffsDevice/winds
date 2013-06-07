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
		
		_callbackReturn refreshHandler( _event event );
		_callbackReturn inputHandler( _event event );
		
		// Refreshes the big colorful base gradient
		void refreshBigGradient();
		
	public:
		
		//! Select a date
		void selectColor( _pixel color );
		void setIntValue( _pixel color ){ this->selectColor( color ); }
		
		//! Get the selected date
		_pixel getSelectedColor() const ;
		_pixel getIntValue() const { return this->getSelectedColor(); }
		
		//! Ctor
		_colorpicker( _length width , _length height , _coord x , _coord y , _pixel initialColor = COLOR_BLUE , _style style = _style() );
		
		//! Dtor
		~_colorpicker();
};

#endif