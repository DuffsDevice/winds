// Check if already included
#ifndef _WIN_G_PROGRESSBAR_
#define _WIN_G_PROGRESSBAR_

#include "_type/type.gadget.h"
#include "_type/type.animation.h"

class _progressbar : public _gadget {
	
	private:
	
		_u8		value;
		bool	type; // 0 = value, 1 = progress
		_animation* anim;
		_u8		state;
		bool	blue;
		
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
		
		void setCol( bool blue ){ this->blue = blue; }
		
		//! Constructor
		_progressbar( _length width , _length height , _coord x , _coord y  , bool type = true , _gadgetStyle style = _defaultStyle_ );
		
		//! Constructor
		_progressbar( _length width , _coord x , _coord y  , bool type = true , _gadgetStyle style = _defaultStyle_ );
		
		//! Destructor
		~_progressbar();
};

#endif