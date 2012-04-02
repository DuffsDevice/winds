// Check if already included
#ifndef _WIN_G_TEXTBOX_
#define _WIN_G_TEXTBOX_

#include "_type/type.gadget.h"
#include "_gadget/gadget.label.h"
#include "interface.input.h"
#include <string>

using namespace std;

class _textbox : public _gadget , public _interface_input {
	
	private:
		
		//! Label to Display Text
		_label* label;
		
		bool pressed;
		
		static _gadgetEventReturnType refreshHandler( _gadgetEvent e );
		static _gadgetEventReturnType mouseHandler( _gadgetEvent e );
		static _gadgetEventReturnType keyHandler( _gadgetEvent e );
		static _gadgetEventReturnType dragHandler( _gadgetEvent e );
		
		// Will be called if the butten is resized ->label will also be resized
		void onResize();
		
	public:
		
		//! Standard Painting Function
		void standardPaint( _gadgetEvent e );
		
		//! Set Text Font
		void setFont( _font* ft ){ if( this->label != nullptr ) this->label->setFont( ft ); }
		
		// Set String-Value
		void setStrValue( string value ){ if( this->label ) this->label->setStrValue( value ); }
		
		// Get String-Value
		string getStrValue(){ if( this->label ) return this->label->getStrValue(); return ""; }
		
		//! Get Text Font
		_font* getFont(){ if( this->label == nullptr ) return nullptr; return this->label->getFont(); }
		
		//! Construcor excluding dimensions and including font
		_textbox( _coord x , _coord y , _length width , string value , _gadgetStyle style = _defaultStyle_ );
		
		//! Destructor
		~_textbox();
};

#endif