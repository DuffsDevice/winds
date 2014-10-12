#ifndef _WIN_G_COLORPICKER_
#define _WIN_G_COLORPICKER_

#include <_type/type.gadget.h>
#include <_gadget/gadget.image.h>

class _gradientColorPicker : public _gadget
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
		
		//! Select a color
		void setColor( _color color );
		
		//! Get the selected color
		_color getColor() const { return _color::fromHSL( this->hue , this->sat , this->lum ); }
		
		//! Ctor
		_gradientColorPicker( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _color initialColor = _color::blue , _style&& style = _style() );
		
		//! Dtor
		~_gradientColorPicker(){
			this->removeChildren( true );
		}
};

#endif