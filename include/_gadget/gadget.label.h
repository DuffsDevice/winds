// Check if already included
#ifndef _WIN_G_LABEL_
#define _WIN_G_LABEL_

#include "_type/type.gadget.h"
#include "interface.input.h"
#include <string>

using namespace std;

class _label : public _gadget , public _interface_input {
	
	private:
	
		friend class _button;
		friend class _checkbox;
		
		//! Farbe der Schrift
		_pixel color;
		_pixel bgColor;
		
		_u8 autoWidth;
		
		//! Schriftart/Font
		//! Default: system-Font inside of _gadgetStyle
		_font* font;
		
		//! Refresh-Handler
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
		
		//! Internal init method to be called inside of all ctors
		void init( string text );
		
		void computeSize();
		
		void onResize(){ this->handleEvent( refresh ); }
		
	public:
		
		void setStrValue( string val );
		
		//! Set Text Color
		void setColor( _pixel col ){ this->color = col; this->handleEvent( refresh ); }
		
		//! Get Text Color
		_pixel getColor(){ return this->color; }
		
		//! Set Text Color
		void setBgColor( _pixel col ){ this->bgColor = col; this->handleEvent( refresh ); }
		
		//! Get Text Color
		_pixel getBgColor(){ return this->bgColor; }
		
		//! Set Text Font
		_font* getFont(){ return this->font; }
		
		//! Refresh on Align-setting
		void setAlign( _align align ){ _interface_input::setAlign( align ); this->refreshBitmap(); }
		
		//! Refresh on Align-setting
		void setVAlign( _valign vAlign ){ _interface_input::setVAlign( vAlign ); this->refreshBitmap(); }
		
		//! Get Text Font
		void setFont( _font* ft );
		
		//! Construcor including dimensions
		_label( _length width , _length height , _coord x , _coord y , string value , _gadgetStyle style = _defaultStyle_ );
		
		//! Construcor excluding dimensions
		_label( _coord x , _coord y , string value , _gadgetStyle style = _defaultStyle_ );
};

#endif