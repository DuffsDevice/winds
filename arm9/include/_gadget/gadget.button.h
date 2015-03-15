// Check if already included
#ifndef _WIN_G_BUTTON_
#define _WIN_G_BUTTON_

#include <_type/type.gadget.h>

class _button : public _gadget
{
	protected:
		
		bool autoSelect;
		
		//! String to be displayed
		wstring 		strValue;
		_fontHandle		font;
		_color			fontColor;
		_fontSize		fontSize;
		bool			pressed;
		
		//! Alignment
		_align			align;
		_valign 		vAlign;
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn mouseHandler( _event );
		static _callbackReturn updateHandler( _event );
		
	public:
		
		//! Align-setting
		void setAlign( _align align ){ if( this->align == align ) return; this->align = align; this->redraw(); }
		
		//! Vertical-Align-setting
		void setVAlign( _valign vAlign ){ if( this->vAlign == vAlign ) return; this->vAlign = vAlign; this->redraw(); }
		
		//! Get Alignment of the Label
		_align getAlign(){ return this->align; }
		
		//! Get Vertical Alignment of the Label
		_valign getVAlign(){ return this->vAlign; }
		
		//! Set Title to be displayed on top of the button
		void setStrValue( wstring val );
		
		//! Get Title of the button
		wstring getStrValue(){ return this->strValue; }
		
		//! Whether the button should be outlined by a blue line
		void setAutoSelect( bool aS ){ if( this->autoSelect == aS ) return; this->autoSelect = aS; this->redraw(); }
		
		//! Whether the button should be outlined by a blue line
		bool isAutoSelect(){ return this->autoSelect; }
		
		//! Set Text Font
		void setFont( _fontHandle ft );
		
		//! Get Text Font
		_fontHandle getFont(){ return this->font; }
		
		//! Get Text FontSize
		_fontSize getFontSize(){ return this->fontSize; }
		
		//! Get Text FontSize
		void setFontSize( _fontSize size );
		
		//! Set Text Color
		void setFontColor( _color col ){ if( this->fontColor == col ) return; this->fontColor = col; this->redraw(); }
		
		//! Get Text Color
		_color getFontColor(){ return this->fontColor; }
		
		//! Constructor with dimsnions, coordinates, title and optional: Style
		_button( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , wstring title = "" , _style&& style = _style() );
};

#endif