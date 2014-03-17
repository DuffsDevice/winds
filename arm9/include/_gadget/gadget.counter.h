#ifndef _WIN_G_COUNTER_
#define _WIN_G_COUNTER_

#include "_type/type.h"
#include "_gadget/gadget.scrollBar.button.h"
#include "_gadget/gadget.label.h"

class _counter : public _gadget{
	
	protected:
		
		bool circular;
		_s32 lowerBound;
		_s32 upperBound;
		_s32 intValue;
		_u8	 decimals;
		_u8	 numbersystem;
		
		_scrollButton*	increaseHandle;
		_scrollButton*	decreaseHandle;
		_label*			valueLabel;
		
		_callbackReturn btnClickHandler( _event );
		static _callbackReturn keyHandler( _event );
		static _callbackReturn focusHandler( _event );
		static _callbackReturn refreshHandler( _event );
		
		void	refreshDecimals(){
			this->decimals = max( countDecimals( this->upperBound , this->numbersystem ) , countDecimals( this->lowerBound , this->numbersystem ) );
		}
		
		//! Increases the counter value and fires an onEdit-Event
		void increaseInternal(){ this->increase(); this->triggerEvent( onEdit ); }
		
		//! Decreases the counter value and fires an onEdit-Event
		void decreaseInternal(){ this->decrease(); this->triggerEvent( onEdit ); }
		
	public:
		
		//! Set the _counter's value
		void	setIntValue( _s32 value );
		
		//! Get the _counter's value
		_s32	getIntValue(){ return this->intValue; }
		
		//! Set the Upper Bound of the valid range
		void setUpperBound( _s32 upperBound )
		{
			this->upperBound = upperBound;
			this->refreshDecimals();
			this->setIntValue( min( this->intValue , this->upperBound ) );
		}
		
		//! Set the Lower Bound of the valid range
		void setLowerBound( _s32 lowerBound )
		{
			this->lowerBound = lowerBound;
			this->refreshDecimals();
			this->setIntValue( max( this->intValue , this->lowerBound ) );
		}
		
		//! Get the Lower Bound of the valid range
		_s32 getLowerBound(){ return this->lowerBound; }
		
		//! Get the Upper Bound of the valid range
		_s32 getUpperBound(){ return this->upperBound; }
		
		//! Increases the counter value
		void increase(){ this->setIntValue( this->intValue + 1 ); }
		
		//! Decreases the counter value
		void decrease(){ this->setIntValue( this->intValue - 1 ); }
		
		//! Ctors
		_counter( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , bool circular = false , _s32 value = 0 , _optValue<_s32> upperBound = ignore , _optValue<_s32> lowerBound = ignore  , _optValue<_u8> numbersystem = ignore  , _style&& style = _style() );
		
		//! Dtor
		~_counter();
};
#endif