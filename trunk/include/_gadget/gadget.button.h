// Check if already included
#ifndef _WIN_G_BUTTON_
#define _WIN_G_BUTTON_

#include "_type/type.gadget.h"
#include "_gadget/gadget.label.h"

class _button : public _gadget {
	
	protected:
		
		//! Label on the top of the button
		_label* label;
		
		bool autoSelect;
		bool pressed;
		
		_u8 	computeW;
		_u8 	computeH;
		
		static _callbackReturn refreshHandler( _event event );
		
		static _callbackReturn mouseHandler( _event event );
		
		static _callbackReturn dragHandler( _event event );
		
		// Will be called if the butten is resized ->label will also be resized
		static _callbackReturn resizeHandler( _event e );
		
		void init( string text );
		
		void computeSize();
		
	public:
		
		//! Set Alignment of the Button
		void setAlign( _align align ){ this->label->setAlign(align); }
		
		//! Get Alignment of the Button
		_align getAlign(){ return this->label->getAlign(); }
		
		//! Set Vertical Alignment of the Button
		void setVAlign( _valign vAlign ){ this->label->setVAlign(vAlign); }
		
		//! Get Vertical Alignment of the Button
		_valign getVAlign(){ return this->label->getVAlign(); }
		
		//! Set Title to be displayed on top of the button
		void setStrValue( string val ){ this->label->setStrValue(val); this->computeSize(); }
		
		//! Get Title of the button
		string getStrValue(){ return this->label->getStrValue(); }
		
		//! Whether the button should be outlined by a blue line
		void setAutoSelect( bool aS );
		
		//! Whether the button should be outlined by a blue line
		bool isAutoSelect();
		
		//! Whether the button is pressed currently (used in painting functions)
		bool isPressed(){ return this->pressed ; }
		
		//! Set Text Font
		void setFont( _font* ft );
		
		//! Get Text Font
		_font* getFont(){ return this->label->getFont(); }
		
		// Methods to set Size
		void setWidth( _u8 width );
		void setDimensions( _rect dim );
		void setHeight( _u8 height );

		// Methods to tell: We want it to compute the Size on its own
		void setWidth();
		void setDimensions();
		void setHeight();
		
		//! Constructor with dimsnions, coordinates, title and optional: Style
		_button( _length width , _length height , _coord x , _coord y , string title = "" , _style style = _style() );
		
		//! Constructor with coordinates, title and optional: Style
		//! The Width will be computed by the font
		_button( _coord x , _coord y , string text = "" , _style style = _style() );
		
		//! Destrucor
		~_button();
};

#endif