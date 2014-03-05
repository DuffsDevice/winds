#ifndef _WIN_G_COLORPICKER_HEXAGON_
#define _WIN_G_COLORPICKER_HEXAGON_

#include "_type/type.gadget.h"
#include "_type/type.color.h"

class _hexagonColorPicker : public _gadget
{
	protected:
		
		//! Currently adjusted components
		_color	color;
		_color	bgColor;
		_s16	hexagonNumber;
		
		_callbackReturn greyscaleRefreshHandler( _event );
		_callbackReturn hexagonRefreshHandler( _event );
		_callbackReturn refreshHandler( _event );
		_callbackReturn inputHandler( _event );
		
		static _2s32 getHexagonPos( _s16 number );
		static _2s32 getHexagonSize( _s16 number );
		static _color getHexagonColor( _s16 number );
		
	public:
		
		//! Select a color
		void setColor( _color color );
		
		//! Get the selected color
		_color getColor() const { return this->color; }
		
		//! Set background color
		void setBgColor( _color bgColor ){ if( this->bgColor == bgColor ) return; this->bgColor = bgColor; this->redraw(); }
		
		//! Get background color
		_color getBgColor() const { return this->bgColor; };
		
		//! Ctor
		_hexagonColorPicker( _optValue<_coord> x , _optValue<_coord> y , _color initialColor = _color::white , _style&& style = _style() );
		
		//! Dtor
		~_hexagonColorPicker(){
			this->removeChildren( true );
		}
};

#endif