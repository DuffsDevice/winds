// Check if already included
#ifndef _WIN_G_TEXTBOX_
#define _WIN_G_TEXTBOX_

#include "_type/type.gadget.h"

class _textbox : public _gadget{
	
	private:
		
		//! Farbe der Schrift
		_pixel 	color;
		_pixel 	bgColor;
		
		//! Schriftart/Font
		//! Default: system-Font inside of _system_->_runtimeAttributes_
		_font* 	font;
		_u8		fontSize;
		
		//! Alignment
		_align	align;
		_valign vAlign;
		
		//! String to be displayed
		string 	strValue;
		
		//! Current cursor position
		_length	cursor;
		
		bool	pressed;
		
		static _callbackReturn refreshHandler( _event e );
		static _callbackReturn focusHandler( _event e );
		static _callbackReturn blurHandler( _event e );
		static _callbackReturn mouseHandler( _event e );
		static _callbackReturn keyHandler( _event e );
		
		static void getFontPosition( _coord& x , _coord& y , _textbox* box );
		
	public:
		
		//! Set the Text to be displayed
		void setStrValue( string val );
		
		//! Get the Text of the label
		string getStrValue(){ return this->strValue; }
		
		//! Get Text Font
		_font* getFont(){ return this->font; }
		
		//! Get Text FontSize
		_u8 getFontSize(){ return this->fontSize; }
		
		//! Set Text Color
		void setColor( _pixel col ){ this->color = col; this->bubbleRefresh( true ); }
		
		//! Get Text Color
		_pixel getColor(){ return this->color; }
		
		//! Set Text Color
		void setBgColor( _pixel col ){ this->bgColor = col; this->bubbleRefresh( true ); }
		
		//! Get Text Color
		_pixel getBgColor(){ return this->bgColor; }
		
		//! Align-setting
		void setAlign( _align align ){ this->align = align; this->bubbleRefresh( true ); }
		
		//! Vertical-Align-setting
		void setVAlign( _valign vAlign ){ this->vAlign = vAlign; this->bubbleRefresh( true ); }
		
		//! Get Alignment of the Label
		_align getAlign(){ return this->align; }
		
		//! Get Vertical Alignment of the Label
		_valign getVAlign(){ return this->vAlign; }
		
		//! Set Text Font
		void setFont( _font* ft ){ if( this->font != ft ){ this->font = ft; this->bubbleRefresh( true ); } }
		
		//! Set FontSize
		void setFontSize( _u8 fontSize ){ if( this->fontSize != fontSize ){ this->fontSize = fontSize; this->bubbleRefresh( true ); } }
		
		//! Construcor excluding dimensions and including font
		_textbox( _coord x , _coord y , _length width , string value = "" , _style style = _style() );
};

#endif