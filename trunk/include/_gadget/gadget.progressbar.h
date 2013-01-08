// Check if already included
#ifndef _WIN_G_PROGRESSBAR_
#define _WIN_G_PROGRESSBAR_

#include "_type/type.gadget.h"
#include "_type/type.animation.h"

class _progressbar : public _gadget {
	
	private:
	
		bool	type; // 0 = value, 1 = progress
		_u8		value;
		_u8		state;
		bool	blue;
		
		static _callbackReturn refreshHandler( _event event );
		
		void	step();
	
	public:
	
		//! Set Receiver of Key-Events
		void setIntValue( _u8 value ){ // 0 - 127
			if( value != this->value )
			{
				this->value = value;
				this->bubbleRefresh( true );
			}
		}
		
		//! getIntValue
		_u8 getIntValue(){ // 0 - 127
			return this->value = value;
		}
		
		//! Whether the bar shows "progress" (=false) or shows the actual state (=true)
		void setBarType( bool type )
		{
			if( type != this->type )
			{
				this->type = type;
				this->bubbleRefresh( true );
			}
		}
		
		//! Get Scrollbar-Type
		bool getBarType(){ return this->type; }
		
		//! set Color Scheme ( true for the blue theme used at the bootup )
		void setColorScheme( bool blue )
		{
			if( this->blue != blue )
			{
				this->blue = blue;
				this->bubbleRefresh( true );
			}
		}
		
		//! get Color Scheme
		bool getColorScheme(){ return this->blue; }
		
		//! Constructor
		_progressbar( _length width , _length height , _coord x , _coord y  , bool type = true , _style style = _style() );
		
		//! Constructor
		_progressbar( _length width , _coord x , _coord y  , bool type = true , _style style = _style() );
		
		//! Destructor
		~_progressbar();
};

#endif