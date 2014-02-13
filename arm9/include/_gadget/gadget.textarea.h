// Check if already included
#ifndef _WIN_G_TEXTAREA_
#define _WIN_G_TEXTAREA_

#include "_type/type.gadget.h"
#include "_gadget/gadget.scrollBar.h"
#include "_type/type.text.h"

class _textArea : public _gadget{
	
	private:
		
		enum
		{
			borderX = 3, // X-Position of the most left character
			borderY = 1, // Start of text in y-direction
		};
		
		//! Farbe der Schrift
		_color 		color;
		_color 		bgColor;
		
		//! _text-object
		_text 		text;
		
		//! Current cursor position
		_u32		cursor;
		_scrollBar*	scrollBar;
		
		//! Text-align
		_align		align;
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn generalHandler( _event );
		static _callbackReturn mouseHandler( _event );
		static _callbackReturn keyHandler( _event );
		
		inline void checkRefresh()
		{
			if( this->text.needsRefresh() )
			{
				this->handleEvent( onResize );//! Set the right parameters for the Scrollbar
				this->redraw();
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
		void setFont( const _font* ft ){
			if( !ft )
				return;
			this->text.setFont( ft );
			this->scrollBar->setStep( this->text.getFont()->getHeight() + 1 );
			this->checkRefresh();
		}
		
		//! Set FontSize
		void setFontSize( _u8 fontSize ){
			this->text.setFontSize( fontSize );
			this->scrollBar->setStep( this->text.getFont()->getHeight() + 1 );
			this->checkRefresh();
		}
		
		//! Set Text Color
		void setColor( _color col ){ this->color = col; this->redraw(); }
		
		//! Get Text Color
		_color getColor(){ return this->color; }
		
		//! Set Text Color
		void setBgColor( _color col ){ this->bgColor = col; this->redraw(); }
		
		//! Get Text Color
		_color getBgColor(){ return this->bgColor; }
		
		//! Align-setting
		void setAlign( _align align ){ if( this->align == align ) return; this->align = align; this->redraw(); }
		
		//! Get Alignment of the Label
		_align getAlign(){ return this->align; }
		
		//! Set The cursor
		void setCursor( _s64 cursor = -1 ){	this->setInternalCursor( max( _s64(0) , cursor + 1 ) ); }
		
		//! Get the current cursor (-1 equals no cursor)
		_s64 getCursor(){ return _s64(this->cursor) - 1; }
		
		
		//! Ctor
		_textArea( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string value = "" , _style&& style = _style() );
		
		//! Dtor
		~_textArea();
};

#endif