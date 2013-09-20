#ifndef _WIN_G_SLIDER_
#define _WIN_G_SLIDER_

#include "_type/type.h"
#include "_type/type.gadget.h"
#include "_gadget/gadget.label.h"
#include "_gadget/gadget.popup.h"
#include "_gadget/gadget.slider.handle.h"

class _slider : public _gadget{
	
	protected:
		
		_s32		intValue;
		float		percentage;
		_s32		lowerBound;
		_s32		upperBound;
		_dimension	dimension;
		_u32		snap;
		
		_sliderHandle*	sliderHandle;
		string			upperBoundText;
		string			lowerBoundText;
		
		_callbackReturn dragHandler( _event event );
		static _callbackReturn keyHandler( _event event );
		static _callbackReturn focusHandler( _event event );
		static _callbackReturn refreshHandler( _event event );
		static _callbackReturn updateHandler( _event event );
		
		//! Increases the slider value and fires an onEdit-Event
		void increaseInternal(){ this->setIntValue( this->intValue + 1 ); this->triggerEvent( onEdit ); }
		
		//! Decreases the slider value and fires an onEdit-Event
		void decreaseInternal(){ this->setIntValue( this->intValue - 1 ); this->triggerEvent( onEdit ); }
		
	public:
		
		//! Set the _slider's value
		void	setIntValue( _s32 value ){
			this->intValue = mid( lowerBound , value , upperBound );
			this->update();
		}
		
		//! Get the _slider's value
		_s32	getIntValue(){ return this->intValue; }
		
		//! Set the Upper Bound of the valid range
		void setUpperBound( _s32 upperBound )
		{
			this->upperBound = upperBound;
			this->setIntValue( min( this->intValue , this->upperBound ) );
			this->update();
		}
		
		//! Set the Lower Bound of the valid range
		void setLowerBound( _s32 lowerBound )
		{
			this->lowerBound = lowerBound;
			this->setIntValue( max( this->intValue , this->lowerBound ) );
			this->update();
		}
		
		//! Get the Lower Bound of the valid range
		_s32 getLowerBound(){ return this->lowerBound; }
		
		//! Get the Upper Bound of the valid range
		_s32 getUpperBound(){ return this->upperBound; }
		
		//! Set whether indicators should be drawn every 'n' values
		void setSnap( _u32 n = 0 ){ if( this->snap == n ) return; this->snap = n; this->redraw(); }
		
		//! Check whether indicators should be drawn
		_u32 getSnap(){ return this->snap; }
		
		//! Set Texts to be displayed at the extremes
		void setLowerLabel( string text ){ if( this->lowerBoundText == text ) return; this->lowerBoundText = text; this->update(); }
		void setUpperLabel( string text ){ if( this->upperBoundText == text ) return; this->upperBoundText = text; this->update(); }
		
		//! ... and getters for them
		string getUpperLabel(){ return this->upperBoundText; }
		string getLowerLabel(){ return this->lowerBoundText; }
		
		//! Increases the slider value
		void increase(){ this->setIntValue( this->intValue + 1 ); }
		
		//! Decreases the slider value
		void decrease(){ this->setIntValue( this->intValue - 1 ); }
		
		//! Basic Ctor
		_slider( _coord x , _coord y , _length sliderLength , _s32 value = 0 , _dimension dimension = _dimension::horizontal , _style&& style = _style() ) :
			_slider( x , y , sliderLength , value , dimension , 4 , 0 , (_style&&)style )
		{ }
		
		//! Full Ctor
		_slider( _coord x , _coord y , _length sliderLength , _s32 value , _dimension dimension , _s32 upperBound , _s32 lowerBound = 0 , _style&& style = _style() );
		
		//! Dtor
		~_slider();
		
};
#endif