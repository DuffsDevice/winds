#ifndef _WIN_G_SCROLLBAR_
#define _WIN_G_SCROLLBAR_

#include "_type/type.gadget.h"
#include "_type/type.animation.h"
#include "_gadget/gadget.button.h"

enum class _dimension : _u8{
	horizontal = 0 ,
	vertical = 3
};

class _scrollBar : public _gadget{
	
	private:
		
		_u32 		value;
		_u32 		length; // Length of the visible Part -> scrollHandle length
		_u32 		length2;// Length of the whole scrollCanvas
		_u32 		cache;  // Will be "Physically scrollbar Height" - "theoratically scrollbarHeight"
							// This will keep the scrollBar in a more visible state
		_u16		step;
		
		_dimension 	dim;
		
		_button* 	dragHandle;
		_button* 	higherHandle;
		_button* 	lowerHandle;
		
		_animation	anim;
		
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn clickHandler( _event event );
		static _callbackReturn resizeHandler( _event e );
		static _callbackReturn dragHandler( _event e );
		
		void refreshHandleWidth();
		void refreshPosition();
	
	public:
		
		_scrollBar( _coord x , _coord y , _u32 gadgetLength , _u32 length , _u32 length2 , _dimension dim , _u32 value = 0 , _style style = _style() );
		
		//! Simply set the value
		//! @note: this triggers a onScroll-Event!
		void setValue( _u32 value );
		
		void setValue( _u32 value , bool ease );
		
		void setDimension( _dimension dim );
		
		void setStep( _u16 value )
		{
			this->step = value;
		}
		
		void setLength( _u32 value );
		
		void setLength2( _u32 value );
		
		_dimension getDimension(){
			return this->dim;
		}
		
		_u32 getValue(){
			return this->value;
		}
		
};

#endif