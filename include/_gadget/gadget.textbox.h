// Check if already included
#ifndef _WIN_G_TEXTBOX_
#define _WIN_G_TEXTBOX_

#include "_type/type.gadget.h"

class _textbox : public _gadget
{
	protected:
		
		enum{
			borderX = 3,
			borderY = 2
		};
		
		//! Farbe der Schrift
		_pixel 			color;
		_pixel 			bgColor;
		
		//! Schriftart/Font
		//! Default: system-Font inside of _system_->_runtimeAttributes_
		const _font* 	font;
		_u8				fontSize;
		
		//! Alignment
		_align			align;
		_valign 		vAlign;
		
		//! String to be displayed
		string 			strValue;
		
		//! Current cursor position
		_u32			cursor;
		_u32			scroll;
		
		static _callbackReturn refreshHandler( _event e );
		static _callbackReturn focusHandler( _event e );
		static _callbackReturn mouseHandler( _event e );
		static _callbackReturn keyHandler( _event e );
		
		//! Set the Internal Cursor
		void setInternalCursor( _u32 cursor );
		
		//! Internal, not private due to _userWrapper-class
		_2s32 getFontPosition( bool noScrollApplied = false );
		
		//! Made virtual because of e.g. _passcodebox
		virtual void removeStr( _int position , _length numChars = 1 );
		virtual void insertStr( _int position , string s );
		
	public:
		
		//! Set string-value
		virtual void setStrValue( string val );
		
		//! Get string-value
		virtual string getStrValue(){ return this->strValue; }
		
		//! Get Text Font
		const _font* getFont(){ return this->font; }
		
		//! Get Text FontSize
		_u8 getFontSize(){ return this->fontSize; }
		
		//! Set Text Font
		void setFont( const _font* ft );
		
		//! Set FontSize
		void setFontSize( _u8 fontSize ){ if( this->fontSize != fontSize ){ this->fontSize = fontSize; this->bubbleRefresh( true ); } }
		
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
		
		//! Set The cursor
		void setCursor( _s64 cursor = -1 ){	this->setInternalCursor( max( _s64(0) , cursor + 1 ) ); }
		
		//! Get the current cursor (-1 equals no cursor)
		_s64 getCursor(){ return _s64(this->cursor) - 1; }
		
		//! Ctor
		_textbox( _coord x , _coord y , _length width , string value = "" , _style&& style = _style() );
		
		//! Ctor
		_textbox( _coord x , _coord y , _length width , _length height , string value = "" , _style&& style = _style() );
};

#endif