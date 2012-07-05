// Check if already included
#ifndef _WIN_G_LABEL_
#define _WIN_G_LABEL_

#include "_type/type.gadget.h"
#include "interface.input.h"

using namespace std;

class _label : public _gadget , public _interface_input {
	
	protected:
	
		friend class _button;
		friend class _checkbox;
		
		//! Farbe der Schrift
		_pixel 	color;
		_pixel 	bgColor;
		
		_u8 	computeW;
		_u8 	computeH;
		
		//! Schriftart/Font
		//! Default: system-Font inside of _system_->_runtimeAttributes_
		_font* 	font;
		_u8		fontSize;
		
		//! Method to cumpute Space-Requirements
		void computeSize();
		
		//! Refresh-Handler
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
		
	public:
		
		//! Set the Text to be displayed
		void setStrValue( string val );
		
		//! Set Text Color
		void setColor( _pixel col ){ this->color = col; this->bubbleRefresh( true ); }
		
		//! Get Text Color
		_pixel getColor(){ return this->color; }
		
		//! Set Text Color
		void setBgColor( _pixel col ){ this->bgColor = col; this->bubbleRefresh( true ); }
		
		//! Get Text Color
		_pixel getBgColor(){ return this->bgColor; }
		
		//! Get Text Font
		_font* getFont(){ return this->font; }
		
		//! Get Text FontSize
		_u8 getFontSize(){ return this->fontSize; }
		
		//! Refresh on Align-setting
		void setAlign( _align align ){ _interface_input::setAlign( align ); this->bubbleRefresh( true ); }
		
		//! Refresh on Align-setting
		void setVAlign( _valign vAlign ){ _interface_input::setVAlign( vAlign ); this->bubbleRefresh( true ); }
		
		//! Set Text Font
		void setFont( _font* ft );
		
		//! Set FontSize
		void setFontSize( _u8 fontSize );
		
		//! Construcor including dimensions
		_label( _length width , _length height , _coord x , _coord y , string value , _gadgetStyle style = _defaultStyle_ );
		
		//! Construcor excluding dimensions
		_label( _coord x , _coord y , string text , _gadgetStyle style = _defaultStyle_ );
		
		// Methods to set Size
		void setWidth( _u8 width );
		void setDimensions( _rect dim );
		void setHeight( _u8 height );
};

#endif