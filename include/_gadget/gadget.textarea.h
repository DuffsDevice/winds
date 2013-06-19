// Check if already included
#ifndef _WIN_G_TEXTAREA_
#define _WIN_G_TEXTAREA_

#include "_type/type.gadget.h"
#include "_gadget/gadget.scrollBar.h"
#include "_type/type.text.h"

class _textarea : public _gadget{
	
	private:
		
		enum
		{
			borderX = 3, // X-Position of the most left character
			borderY = 1, // Start of text in y-direction
		};
		
		//! Farbe der Schrift
		_pixel 		color;
		_pixel 		bgColor;
		
		//! _text-object
		_text 		text;
		
		//! Current cursor position
		_length		cursor;
		_scrollBar*	scrollBar;
		
		//! Text-align
		_align		align;
		
		static _callbackReturn refreshHandler( _event e );
		static _callbackReturn generalHandler( _event e );
		static _callbackReturn mouseHandler( _event e );
		static _callbackReturn keyHandler( _event e );
		
		inline void checkRefresh()
		{
			if( this->text.needsRefresh() )
			{
				this->handleEvent( onResize );//! Set the right parameters for the Scrollbar
				this->bubbleRefresh( true );
			}
		}
		
		//! Set the Internal Cursor
		void setInternalCursor( _u32 cursor );
		
	public:
		
		//! Get the font Position, where the text will be painted at
		_2s32 getFontPosition( string str , bool noScrollApplied = false );
		
		//! Set the Text to be displayed
		void setStrValue( string val ){ this->text.setText( val ); this->checkRefresh(); }
		
		//! Get the Text of the textbox
		string getStrValue(){ return this->text.getText(); }
		
		//! Get Text Font
		const _font* getFont(){ return this->text.getFont(); }
		
		//! Get Text FontSize
		_u8 getFontSize(){ return this->text.getFontSize(); }
		
		//! Set Text Font
		void setFont( const _font* ft ){ this->text.setFont( ft ); this->scrollBar->setStep( this->text.getFont()->getHeight() + 1 ); this->checkRefresh(); }
		
		//! Set FontSize
		void setFontSize( _u8 fontSize ){ this->text.setFontSize( fontSize ); this->scrollBar->setStep( this->text.getFont()->getHeight() + 1 ); this->checkRefresh(); }
		
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
		
		//! Set The cursor
		void setCursor( _s64 cursor = -1 ){	this->setInternalCursor( max( _s64(0) , cursor + 1 ) ); }
		
		//! Get the current cursor (-1 equals no cursor)
		_s64 getCursor(){ return _s64(this->cursor) - 1; }
		
		
		//! Ctor
		_textarea( _length width , _length height , _coord x , _coord y , string value = "" , _style&& style = _style() );
		
		//! Dtor
		~_textarea();
};

#endif