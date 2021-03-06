#ifndef _WIN_G_SCROLLBAR_
#define _WIN_G_SCROLLBAR_

#include <_type/type.gadget.h>
#include <_type/type.animation.h>
#include <_gadget/gadget.scrollBar.button.h>

class _scrollBar : public _gadget {
	
	private:
		
		_u32 		value;
		_u32 		length; // Length of the visible Part -> scrollHandle length
		_u32 		length2;// Length of the whole scrollCanvas
		_u32 		cache;  // Will be "Physically scrollbar Height" - "theoratically scrollbarHeight"
							// This will keep the scrollBar in a more visible state
		_u16		step;
		
		_dimension 	dim;
		
		_scrollButton* 	dragHandle;
		_scrollButton* 	higherHandle;
		_scrollButton* 	lowerHandle;
		
		_animation	anim;
		
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn clickHandler( _event event );
		static _callbackReturn resizeHandler( _event e );
		static _callbackReturn dragHandler( _event e );
		
		void refreshHandleWidth();
		void refreshPosition();
		
		void setValueInternal( int value );
	
	public:
		
		//! Ctor
		_scrollBar( _optValue<_coord> x , _optValue<_coord> y , _length gadgetLength , _u32 length , _u32 length2 , _dimension dim , _u32 value = 0 , _style&& style = _style() );
		
		//! Dtor
		~_scrollBar();
		
		//! This one doesn't
		void setValue( _u32 value , bool ease );
		
		_u32 getValue(){ return this->value; }
		
		void setDimension( _dimension dim );
		_dimension getDimension(){ return this->dim; }
		
		void setStep( _u16 value ){ this->step = value; }
		_u16 getStep(){ return this->step; }
		
		void setLength( _u32 value );
		_u32 getLength(){ return this->length; }
		
		void setLength2( _u32 value );
		_u32 getLength2(){ return this->length2; }
		
};

extern _fromStr<_dimension>	string2dimension;
extern _toStr<_dimension>	dimension2string;

#endif