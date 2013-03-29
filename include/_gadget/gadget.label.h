// Check if already included
#ifndef _WIN_G_LABEL_
#define _WIN_G_LABEL_

#include "_type/type.gadget.h"

class _label : public _gadget {
	
	private:
		
		//! Farbe der Schrift
		_pixel			color;
		_pixel			bgColor;
		
		_u8 			computeW;
		_u8 			computeH;
		
		//! Schriftart/Font
		//! Default: system-Font inside of _system_->_runtimeAttributes_
		const _font* 	font;
		_u8				fontSize;
		
		//! Alignment
		_align			align;
		_valign 		vAlign;
		
		//! String to be displayed
		string 			strValue;
		
		//! Method to cumpute Space-Requirements
		void computeSize();
		
		//! Refresh-Handler
		static _callbackReturn refreshHandler( _event event );
		
	public:
		
		//! Set the Text to be displayed
		void setStrValue( string val );
		
		//! Get the Text of the label
		string getStrValue(){ return this->strValue; }
		
		//! Set Text Color
		void setColor( _pixel col ){ this->color = col; this->bubbleRefresh( true ); }
		
		//! Get Text Color
		_pixel getColor(){ return this->color; }
		
		//! Set Text Color
		void setBgColor( _pixel col ){ this->bgColor = col; this->bubbleRefresh( true ); }
		
		//! Get Text Color
		_pixel getBgColor(){ return this->bgColor; }
		
		//! Get Text Font
		const _font* getFont(){ return this->font; }
		
		//! Get Text FontSize
		_u8 getFontSize(){ return this->fontSize; }
		
		//! Align-setting
		void setAlign( _align align ){ this->align = align; this->bubbleRefresh( true ); }
		
		//! Vertical-Align-setting
		void setVAlign( _valign vAlign ){ this->vAlign = vAlign; this->bubbleRefresh( true ); }
		
		//! Get Alignment of the Label
		_align getAlign(){ return this->align; }
		
		//! Get Vertical Alignment of the Label
		_valign getVAlign(){ return this->vAlign; }
		
		//! Set Text Font
		void setFont( const _font* ft );
		
		//! Set FontSize
		void setFontSize( _u8 fontSize );
		
		//! Construcor including dimensions
		_label( _length width , _length height , _coord x , _coord y , string value , _style style = _style() | _styleAttr::notClickable );
		
		//! Construcor excluding dimensions
		_label( _coord x , _coord y , string text , _style style = _style() | _styleAttr::notClickable );
		
		// Methods to set Size
		void setWidth( _u8 width );
		void setDimensions( _rect dim );
		void setHeight( _u8 height );
};

#endif