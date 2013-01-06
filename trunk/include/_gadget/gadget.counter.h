#ifndef _WIN_G_COUNTER_
#define _WIN_G_COUNTER_

#include "_type/type.h"
#include "_gadget/gadget.scrollBar.button.h"

class _counter : public _gadget{
	
	protected:
		
		bool circular;
		_s32 intValue;
		_s32 lowerBound;
		_s32 upperBound;
		_u8	 decimals;
		_u8	 numbersystem;
		
		void increase(){ this->setIntValue( this->intValue + 1 ); }
		void decrease(){ this->setIntValue( this->intValue - 1 ); }
		
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
		
		void	setIntValue( int value );
		
		_s32	getIntValue(){ return this->intValue; }
		
		void setUpperBound( _s32 upperBound )
		{
			this->upperBound = upperBound;
			refreshDecimals();
		}
		
		void setLowerBound( _s32 lowerBound )
		{
			this->lowerBound = lowerBound;
			refreshDecimals();
		}
		
		_s32 getLowerBound(){ return this->lowerBound; }
		
		_s32 getUpperBound(){ return this->upperBound; }
		
		_counter( _coord x , _coord y , _length width , bool circular = false , _s32 value = 0 , _style style = _style() );
		_counter( _coord x , _coord y , _length width , bool circular , _s32 value , _s32 upperBound , _s32 lowerBound = 0 , _u8 numbersystem = 10 , _style style = _style() );
		
};
#endif