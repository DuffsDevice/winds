// Check if already included
#ifndef _WIN_G_TEXTAREA_
#define _WIN_G_TEXTAREA_

#include "_type/type.gadget.h"
#include "_gadget/gadget.scrollBar.h"
#include "_type/type.guistring.multiline.h"

class _textArea : public _gadget{
	
	private:
		
		enum
		{
			borderX = 3, // X-Position of the most left character
			borderY = 1, // Start of text in y-direction
		};
		
		//! background color
		_color 				bgColor;
		
		//! _guiString-object
		_multiLineGuiString text;
		
		//! Current cursor position
		_scrollBar*			scrollBar;
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn generalHandler( _event );
		static _callbackReturn mouseHandler( _event );
		static _callbackReturn keyHandler( _event );
		
		// Checks, if the text wants to be refreshed, in case something changed concerning the appearence of the text
		// in case, that is the case, it refreshes the textarea
		void checkRefresh(){
			if( this->text.needsRefresh() ){
				this->handleEvent( onResize );	// Sets the right parameters for the Scrollbar
				this->redraw();					// This will redraw the text
			}
		}
		
		// Checks, if the text wants to update its wrapping of lines and does that if needed
		void checkUpdate(){
			if( this->text.needsUpdate() )
				this->text.update( this->getGuiStringDimensions() );
		}
		
		// Get The current dimensions of the guistring
		_rect getGuiStringDimensions() const {
			return _rect(
				_textArea::borderX
				, _textArea::borderY - this->scrollBar->getValue()
				, this->getWidth() - _textArea::borderX * 2 - ( this->scrollBar->isHidden() ? 0 : 9 )
				, this->getHeight() - _textArea::borderY * 2
			);
		}
		
		//! Update the scroll so that the current cursor gets in view
		void adjustScrollToCursor();
		
	public:
		
		//! Set the Text to be displayed
		void setStrValue( string val ){ this->text = move(val); this->checkRefresh(); }
		
		//! Get the Text of the textbox
		string getStrValue(){ return this->text; }
		
		//! Get Text Font
		_fontHandle getFont(){ return this->text.getFont(); }
		
		//! Get Text FontSize
		_u8 getFontSize(){ return this->text.getFontSize(); }
		
		//! Set Text Font
		void setFont( _fontHandle ft ){
			if( !ft )
				return;
			this->text.setFont( ft );
			this->checkRefresh();
		}
		
		//! Set FontSize
		void setFontSize( _u8 fontSize ){
			this->text.setFontSize( fontSize );
			this->checkRefresh();
		}
		
		//! Set Text Color
		void setColor( _color col ){ this->text.setFontColor( col ); this->checkRefresh(); }
		
		//! Get Text Color
		_color getColor(){ return this->text.getFontColor(); }
		
		//! Set Text Color
		void setBgColor( _color col ){ this->bgColor = col; this->redraw(); }
		
		//! Get Text Color
		_color getBgColor(){ return this->bgColor; }
		
		//! Align-setting
		void setAlign( _align align ){ this->text.setAlign( align ); this->checkRefresh(); }
		
		//! Get Alignment of the Label
		_align getAlign(){ return this->text.getAlign(); }
		
		//! Set The cursor
		void setCursor( _optValue<_u32> cursor = ignore );
		
		//! Get the current cursor
		_u32 getCursor(){ return this->text.getCursor(); }
		
		
		//! Ctor
		_textArea( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string value = "" , _style&& style = _style() );
		
		//! Dtor
		virtual ~_textArea();
};

#endif