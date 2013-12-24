// Check if already included
#ifndef _WIN_G_PROGRESSBAR_
#define _WIN_G_PROGRESSBAR_

#include "_type/type.gadget.h"
#include "_type/type.timer.h"

class _progressBar : public _gadget {
	
	private:
	
		bool	type; // 0 = value, 1 = progress
		_u8		value;
		_u8		state;
		bool	blue;
		
		static _callbackReturn refreshHandler( _event );
		
		void	step();
		_timer	timer;
	
	public:
	
		//! Constructor
		_progressBar( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , bool type = true , _style&& style = _style() | _styleAttr::notClickable );
		
		//! Set Receiver of Key-Events
		void setIntValue( _u8 value ){ // 0 - 127
			if( value == this->value )
				return;
			this->value = value;
			this->redraw();
		}
		
		//! getIntValue
		_u8 getIntValue(){ // 0 - 127
			return this->value = value;
		}
		
		//! Whether the bar shows "progress" (=false) or shows the actual state (=true)
		void setBarType( bool type );
		
		//! Get Scrollbar-Type
		bool getBarType(){ return this->type; }
		
		//! set Color Scheme ( true for the blue theme used at the bootup )
		void setColorScheme( bool blue )
		{
			if( this->blue == blue )
				return;
			this->blue = blue;
			this->redraw();
		}
		
		//! get Color Scheme
		bool getColorScheme(){ return this->blue; }
};

#endif