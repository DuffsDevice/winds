// Check if already included
#ifndef _WIN_G_LABEL_
#define _WIN_G_LABEL_

#include "_type/type.gadget.h"
#include "interface.input.h"
#include <string>

using namespace std;

class _label : public _gadget , public _interface_input {
	
	private:
	
		friend class _button;
		friend class _checkbox;
		
		//! Farbe der Schrift
		_pixel 	color;
		_pixel 	bgColor;
		
		_u8 	computeW;
		_u8 	computeH;
		
		//! Schriftart/Font
		//! Default: system-Font inside of _defaultRuntimeAttributes_
		_font* 	font;
		
		//! Refresh-Handler
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
		
		//! Method to cumpute Space-Requirements
		void computeSize();
		
	public:
		
		//! Set the Text to be displayed
		void setStrValue( string val );
		
		//! Set Text Color
		void setColor( _pixel col ){ this->color = col; this->handleEvent( refresh ); }
		
		//! Get Text Color
		_pixel getColor(){ return this->color; }
		
		//! Set Text Color
		void setBgColor( _pixel col ){ this->bgColor = col; this->handleEvent( refresh ); }
		
		//! Get Text Color
		_pixel getBgColor(){ return this->bgColor; }
		
		//! Set Text Font
		_font* getFont(){ return this->font; }
		
		//! Refresh on Align-setting
		void setAlign( _align align ){ _interface_input::setAlign( align ); this->refreshBitmap(); }
		
		//! Refresh on Align-setting
		void setVAlign( _valign vAlign ){ _interface_input::setVAlign( vAlign ); this->refreshBitmap(); }
		
		//! Get Text Font
		void setFont( _font* ft );
		
		//! Construcor including dimensions
		_label( _length width , _length height , _coord x , _coord y , string value , _gadgetStyle style = _defaultStyle_ );
		
		//! Construcor excluding dimensions
		_label( _coord x , _coord y , string text , _gadgetStyle style = _defaultStyle_ );
		
		// Methods to set Size
		void setWidth( _u8 width );
		void setDimensions( _rect dim );
		void setHeight( _u8 height );

		// Methods to tell: We want it to compute the Size on its own
		void setWidth();
		void setDimensions();
		void setHeight();
};

#endif