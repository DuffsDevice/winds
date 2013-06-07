#ifndef _WIN_G_CALENDAR_
#define _WIN_G_CALENDAR_

#include "_type/type.time.h"
#include "_type/type.gadget.h"
#include "_gadget/gadget.stickybutton.h"
#include "_gadget/gadget.label.h"

class _calendar : public _gadget , private _singleValueGroup<_stickybutton>
{
	protected:
		
		_time		selectedDate;
		
		_u8			curMonth;
		_u16		curYear;
		
		// Will generate the calendar-buttons
		void populateGUI();
		
		// Month-label and month-switcher-buttons
		_label* 	monthLabel;
		_button*	leftArrow;
		_button*	rightArrow;
		_button*	resetButton;
		_button*	todayButton;
		
		_callbackReturn handler( _event event );
		
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
		
		
		//! Ctor with _time object
		_calendar( _length width , _length height , _coord x , _coord y , _time origDate , _style style = _style() ) :
			_calendar( width , height , x , y , origDate.get( _timeAttr::year ) , origDate.get( _timeAttr::month ) , origDate.get( _timeAttr::day ) , style )
		{ }
		
		_calendar( _length width , _length height , _coord x , _coord y , _u16 year , _u8 month , _u8 dayOfMonth , _style style = _style() );
		
		//! Dtor
		~_calendar();
};

#endif