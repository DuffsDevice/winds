// Check if already included
#ifndef _WIN_G_TEXTAREA_
#define _WIN_G_TEXTAREA_

#include "_type/type.gadget.h"
#include "_type/type.text.h"

class _textarea : public _gadget{
	
	private:
		
		enum
		{
			borderX = 3, // X-Position of the most left character
			borderY = 1, // Start of text in y-direction
		};
		
		//! Farbe der Schrift
		_pixel 	color;
		_pixel 	bgColor;
		
		//! _text-object
		_text 	text;
		
		//! Current cursor position
		_length	cursor;
		
		//! Text-align
		_align	align;
		
		static _callbackReturn refreshHandler( _event e );
		static _callbackReturn generalHandler( _event e );
		static _callbackReturn mouseHandler( _event e );
		static _callbackReturn keyHandler( _event e );
		
	public:
		
		static inline _coord getFontPosition( _align align , string str , _font* font , _u16 width )
		{
			switch( align )
			{
				case _align::right:		return width - font->getStringWidth( str ) - _textarea::borderX + 1;
				case _align::center:	return ( width - font->getStringWidth( str ) + 1 ) >> 1;
				default:
				case _align::left:		return _textarea::borderX;
			}
		}
		
		//! Set the Text to be displayed
		void setStrValue( string val ){ this->text.setText( val ); if( this->text.needsRefresh() ) this->bubbleRefresh( true ); }
		
		//! Get the Text of the label
		string getStrValue(){ return this->text.getText(); }
		
		//! Get Text Font
		_font* getFont(){ return this->text.getFont(); }
		
		//! Get Text FontSize
		_u8 getFontSize(){ return this->text.getFontSize(); }
		
		//! Set Text Font
		void setFont( _font* ft ){ this->text.setFont( ft ); if( this->text.needsRefresh() ) this->bubbleRefresh( true ); }
		
		//! Set FontSize
		void setFontSize( _u8 fontSize ){ this->text.setFontSize( fontSize ); if( this->text.needsRefresh() ) this->bubbleRefresh( true ); }
		
		//! Set Text Color
		void setColor( _pixel col ){ this->color = col; this->bubbleRefresh( true ); }
		
		//! Get Text Color
		_pixel getColor(){ return this->color; }
		
		//! Set Text Color
		void setBgColor( _pixel col ){ this->bgColor = col; this->bubbleRefresh( true ); }
		
		//! Get Text Color
		_pixel getBgColor(){ return this->bgColor; }
		
		//! Align-setting
		void setAlign( _align align ){ if( this->align == align ) return; this->align = align; this->bubbleRefresh( true ); }
		
		//! Get Alignment of the Label
		_align getAlign(){ return this->align; }
		
		
		//! Ctor
		_textarea( _length width , _length height , _coord x , _coord y , string value = "" , _style style = _style() );
};

#endif