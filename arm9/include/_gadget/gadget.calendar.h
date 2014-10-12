#ifndef _WIN_G_CALENDAR_
#define _WIN_G_CALENDAR_

#include <_type/type.time.h>
#include <_type/type.gadget.h>
#include <_gadget/gadget.button.sticky.h>
#include <_gadget/gadget.label.h>

class _calendar : public _gadget , private _singleValueGroup<_stickyButton>
{
	protected:
		
		_time		selectedDate;
		
		_u8			curMonth;
		_u16		curYear;
		_u8			lastCurMonth;
		_u16		lastCurYear;
		_color		bgColor;
		
		// Month-label and month-switcher-buttons
		_label* 	monthLabel;
		_button*	leftArrow;
		_button*	rightArrow;
		_button*	resetButton;
		_button*	todayButton;
		
		_callbackReturn clickHandler( _event );
		_callbackReturn updateHandler( _event );
		static _callbackReturn refreshHandler( _event );
		_callbackReturn keyHandler( _event );
		
		// Compute the height of the toppest row, that owns both
		// month-label and the buttons for switching between months
		_int getMonthSelectorHeight() const { return max( 10 , this->getHeight() / 8 + 3 ); }
		
		// Get width of both left&right arrows that change the month
		_int getArrowWidth() const { return max( 9 , this->getWidth() / 10 ); }
		
		_u8 getWeeksInMonth( _time firstDay , _u32 daysInMonth ) const ;
		
	public:
		
		//! Select a date
		void selectDate( _time date ){ this->selectDate( date.get( _timeAttr::year ) , date.get( _timeAttr::month ) , date.get( _timeAttr::day ) ); }
		void selectDate( _u16 year , _u8 month , _u8 dayOfMonth );
		void setIntValue( _int unixTime ){ this->selectDate( _time( unixTime ) ); }
		
		//! Get the selected date
		_time getSelectedDate() const { return this->getIntValue(); }
		_int getIntValue() const { return (_int)this->selectedDate; }
		
		//! Set background color of the calendar
		void setBgColor( _color color ){
			if( this->bgColor == color )
				return;
			this->bgColor = color;
			this->redraw();
		}
		
		//! ..and a getter for it
		_color getBgColor(){ return this->bgColor; }
		
		//! Increases the currently shown month
		void increaseMonth()
		{
			if( this->curMonth < 12 )
				this->curMonth++;
			else{
				this->curYear++;
				this->curMonth = 1;
			}
			this->update(); // Refresh GUI
		}
		
		//! Decreases the currently shown month
		void decreaseMonth()
		{
			if( this->curMonth > 1 )
				this->curMonth--;
			else{
				this->curYear--;
				this->curMonth = 12;
			}
			this->update(); // Refresh GUI
		}
		
		
		//! Ctor with _time object
		_calendar( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _time origDate , _color bgColor = _color::white , _style&& style = _style() ) :
			_calendar( x , y , width , height , origDate.get( _timeAttr::year ) , origDate.get( _timeAttr::month ) , origDate.get( _timeAttr::day ) , bgColor , (_style&&)style )
		{ }
		
		_calendar( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _u16 year , _u8 month , _u8 dayOfMonth , _color bgColor = _color::white , _style&& style = _style() );
		
		//! Dtor
		~_calendar();
};

#endif