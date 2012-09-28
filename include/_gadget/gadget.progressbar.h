// Check if already included
#ifndef _WIN_G_PROGRESSBAR_
#define _WIN_G_PROGRESSBAR_

#include "_type/type.gadget.h"

class _progressbar : public _gadget {
	
	private:
	
		_u8		value;
		bool	type;
		
		static _gadgetEventReturnType refreshHandler( _gadgetEvent event );
	
	public:
	
		//! Set Receiver of Key-Events
		void setValue( _u8 value  ){
			if( value != this->value )
			{
				this->value = value;
				this->bubbleRefresh( true );
			}
		}
		
		//! Whether the bar shows "progress" (=false) or shows the actual state (=true)
		void setType( bool type )
		{
			if( type != this->type )
			{
				this->type = type;
				this->bubbleRefresh( true );
			}
		}
		
		//! Constructor with dimsnions, coordinates, title and optional: Style
		_progressbar( _coord x , _coord y , _length width , bool type = true , _gadgetStyle style = _defaultStyle_ );
};

#endif