#ifndef _WIN_G_COUNTER_
#define _WIN_G_COUNTER_

#include "_type/type.h"
#include "_gadget/gadget.scrollBar.button.h"
#include "_gadget/gadget.label.h"

class _counter : public _gadget{
	
	protected:
		
		bool circular;
		_s32 intValue;
		_s32 lowerBound;
		_s32 upperBound;
		_u8	 decimals;
		_u8	 numbersystem;
		
		_scrollButton*	increaseHandle;
		_scrollButton*	decreaseHandle;
		_label*			valueLabel;
		
		static _callbackReturn changeHandler( _event event );
		static _callbackReturn refreshHandler( _event event );
		
		void refreshDecimals()
		{
			this->decimals = max( countDecimals( this->upperBound , this->numbersystem ) , countDecimals( this->lowerBound , this->numbersystem ) );
		}
		
	public:
		
		//! Set the _counter's value
		void	setIntValue( int value );
		
		//! Get the _counter's value
		_s32	getIntValue(){ return this->intValue; }
		
		//! Set the Upper Bound of the valid range
		void setUpperBound( _s32 upperBound )
		{
			this->upperBound = upperBound;
			refreshDecimals();
		}
		
		//! Set the Lower Bound of the valid range
		void setLowerBound( _s32 lowerBound )
		{
			this->lowerBound = lowerBound;
			refreshDecimals();
		}
		
		//! Get the Lower Bound of the valid range
		_s32 getLowerBound(){ return this->lowerBound; }
		
		//! Get the Upper Bound of the valid range
		_s32 getUpperBound(){ return this->upperBound; }
		
		//! Increase the counter value
		void increase(){ this->setIntValue( this->intValue + 1 ); }
		
		//! Decrease the counter value
		void decrease(){ this->setIntValue( this->intValue - 1 ); }
		
		// Ctors
		_counter( _coord x , _coord y , _length width , bool circular , _s32 value , _style&& style ) :
			_counter( x , y , width , circular , value , 99 , 0 , 10 , (_style&&)style )
		{ }
		_counter( _coord x , _coord y , _length width , bool circular , _s32 value , _s32 upperBound , _s32 lowerBound = 0 , _u8 numbersystem = 10 , _style&& style = _style() );
		
		// Dtor
		~_counter();
		
};
#endif