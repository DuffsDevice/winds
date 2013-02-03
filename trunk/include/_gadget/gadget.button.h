// Check if already included
#ifndef _WIN_G_BUTTON_
#define _WIN_G_BUTTON_

#include "_type/type.gadget.h"

class _button : public _gadget {
	
	protected:
		
		bool autoSelect;
		
		//! String to be displayed
		string 	strValue;
		_font* 	font;
		_pixel	fontColor;
		_u8		fontSize;
		
		//! Alignment
		_align	align;
		_valign vAlign;
		
		_u8 	computeW;
		_u8 	computeH;
		
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn mouseHandler( _event event );
		static _callbackReturn dragHandler( _event event );
		
		void init( string text );
		
		void computeSize();
		
	public:
		
		//! Align-setting
		void setAlign( _align align ){ this->align = align; this->bubbleRefresh( true ); }
		
		//! Vertical-Align-setting
		void setVAlign( _valign vAlign ){ this->vAlign = vAlign; this->bubbleRefresh( true ); }
		
		//! Get Alignment of the Label
		_align getAlign(){ return this->align; }
		
		//! Get Vertical Alignment of the Label
		_valign getVAlign(){ return this->vAlign; }
		
		//! Set Title to be displayed on top of the button
		void setStrValue( string val );
		
		//! Get Title of the button
		string getStrValue(){ return this->strValue; }
		
		//! Whether the button should be outlined by a blue line
		void setAutoSelect( bool aS );
		
		//! Whether the button should be outlined by a blue line
		bool isAutoSelect();
		
		//! Set Text Font
		void setFont( _font* ft );
		
		//! Get Text Font
		_font* getFont(){ return this->font; }
		
		//! Get Text FontSize
		_u8 getFontSize(){ return this->fontSize; }
		
		//! Get Text FontSize
		void setFontSize( _u8 size );
		
		//! Set Text Color
		void setFontColor( _pixel col ){ this->fontColor = col; this->bubbleRefresh( true ); }
		
		//! Get Text Color
		_pixel getFontColor(){ return this->fontColor; }
		
		// Methods to set Size
		void setWidth( _length width );
		void setDimensions( _rect dim );
		void setHeight( _length height );
		
		//! Constructor with dimsnions, coordinates, title and optional: Style
		_button( _length width , _length height , _coord x , _coord y , string title = "" , _style style = _style() );
		
		//! Constructor with coordinates, title and optional: Style
		//! The Width will be computed by the font
		_button( _coord x , _coord y , string text = "" , _style style = _style() );
};

#endif