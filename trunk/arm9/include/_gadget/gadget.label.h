// Check if already included
#ifndef _WIN_G_LABEL_
#define _WIN_G_LABEL_

#include <_type/type.gadget.h>
#include <_type/type.guistring.singleline.h>

class _label : public _gadget {
	
	private:
		
		//! Background-color
		_color					bgColor;
		
		//! Gui-string-object
		_singleLineGuiString	text;
		
		//! Refresh-Handler
		static _callbackReturn refreshHandler( _event );
		static _callbackReturn updateHandler( _event );
		
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
				this->text.update( this->getDimensions() );
		}
		
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
		
		//! Vertical-Align-setting
		void setVAlign( _valign vAlign ){ this->text.setVAlign( vAlign ); this->checkRefresh(); }
		
		//! Get vertical Alignment of the Label
		_valign getVAlign(){ return this->text.getVAlign(); }
		
		//! Set ellipsis type
		void setEllipsis( bool enabled , _letterNum lettersBeforeEnd = 0 ){
			lettersBeforeEnd = min<_letterNum>( lettersBeforeEnd , 127 ); // Prevent overflows
			this->text.setEllipsis( enabled ? lettersBeforeEnd : -1 );
			this->checkRefresh();
		}
		
		//! Get (type,lettersBeforeEnd) of ellipsis
		_pair<bool,_s8> getEllipsis() const {
			_s8 ellipsis = this->text.getEllipsis();
			return { ellipsis >= 0 , max<_s8>( 0 , ellipsis ) };
		}
		
		
		//! Construcor including dimensions
		_label( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , string value , _style&& style = _style::notClickable );
};

#endif