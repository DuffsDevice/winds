// Check if already included
#ifndef _WIN_G_TEXTBOX_
#define _WIN_G_TEXTBOX_

#include "_type/type.gadget.h"
#include "_type/type.guistring.singleline.h"

class _textBox : public _gadget
{
	protected:
		
		enum{
			borderX = 3,
			borderY = 2
		};
		
		//! Color of the background
		_color 					bgColor;
		
		//! Current scroll position
		_s32					scroll;
		
		//! _guiString object that will display the text
		_singleLineGuiString	text;
		
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn focusHandler( _event );
		static _callbackReturn mouseHandler( _event );
		static _callbackReturn keyHandler( _event );
		static _callbackReturn updateHandler( _event );
		static _callbackReturn resizeHandler( _event );
		
		//! Set the Internal Cursor
		void			setInternalCursor( _u32 cursor , bool displayCursor );
		
		//! Makes sure that the scroll is set to a value where the cursor is in the visible area
		//! Returns whether the function refreshed the textbox
		bool			makeSureCursorIsVisible();
		
		//! Made virtual because of e.g. _passcodeBox
		virtual void	removeStr( _int position , _length numChars = 1 );
		virtual void	insertStr( _int position , string s );
		
		//! Checks if the textbox should be redrawn
		void			checkRefresh(){
			if( this->text.needsRefresh() ){
				this->handleEvent( onUpdate );
				this->redraw();
			}
		}
		
		//! Checks, if the text wants to update its data
		void checkUpdate(){
			if( this->text.needsUpdate() )
				this->text.update( this->getGuiStringDimensions() );
		}
		
		//! Get the current _rect where the guistring should be
		_rect getGuiStringDimensions() const ;
		
	public:
		
		//! Set string-value
		virtual void	setStrValue( string val ){ this->text = move(val); this->checkRefresh(); }
		
		//! Get string-value
		virtual string	getStrValue() const { return this->text; }
		
		//! Get Text Font
		_fontHandle		getFont() const { return this->text.getFont(); }
		
		//! Get Text FontSize
		_u8				getFontSize() const { return this->text.getFontSize(); }
		
		//! Set Text Font
		void			setFont( _fontHandle ft ){
			if( !ft )
				return;
			this->text.setFont( ft );
			this->checkRefresh();
		}
		
		//! Set FontSize
		void			setFontSize( _u8 fontSize ){
			this->text.setFontSize( fontSize );
			this->checkRefresh();
		}
		
		
		//! Set Text Color
		void			setColor( _color col ){ this->text.setFontColor( col ); this->checkRefresh(); }
		
		//! Get Text Color
		_color			getColor() const { return this->text.getFontColor(); }
		
		
		//! Set Text Color
		void			setBgColor( _color col ){ this->bgColor = col; this->redraw(); }
		
		//! Get Text Color
		_color			getBgColor() const { return this->bgColor; }
		
		
		//! Align-setting
		void			setAlign( _align align ){ this->text.setAlign( align ); this->checkRefresh(); }
		
		//! Get Alignment of the Label
		_align			getAlign() const { return this->text.getAlign(); }
		
		
		//! Vertical-Align-setting
		void			setVAlign( _valign vAlign ){ this->text.setVAlign( vAlign ); this->checkRefresh(); }
		
		//! Get Vertical Alignment of the Label
		_valign			getVAlign() const { return this->text.getVAlign(); }
		
		
		//! Set The cursor
		void			setCursor( _optValue<_u32> cursor = ignore ){ this->setInternalCursor( cursor , cursor.isValid() ); }
		
		//! Get the current cursor
		_u32			getCursor(){ return this->text.getCursor(); }
		
		
		//! Ctor
		_textBox( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string value = "" , _style&& style = _style() );
};

#endif