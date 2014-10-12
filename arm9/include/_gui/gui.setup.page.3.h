#ifndef _WIN_G_SETUP_PAGE3_
#define _WIN_G_SETUP_PAGE3_

#include <_type/type.h>
#include <_type/type.view.h>
#include <_type/type.view.switcher.h>
#include <_type/type.timer.h>
#include <_type/type.gadget.h>
#include <_type/type.gadget.helpers.h>

#include <_gadget/gadget.label.h>
#include <_gadget/gadget.clock.h>
#include <_gadget/gadget.counter.h>
#include <_gadget/gadget.radio.h>
#include <_gadget/gadget.calendar.h>
#include <_type/type.singlevaluegroup.h>
#include <_type/type.time.h>

class _guiSetupPage3 : public _view
{
	private:
	
		_label*						lblTitle;
		_label*						lblAdjustSystemClock;
		_label*						lblFetchTime1;
		_label*						lblFetchTime2;
		_clockGadget*				imgClock;
		_calendar*					calendar;
		_counter*					cntSeconds;
		_counter*					cntMinutes;
		_counter*					cntHours;
		_radio*						radManual;
		_radio*						radAuto;
		_singleValueGroup<_radio>*	radioGroup;
		_label*						colon1;
		_label*						colon2;
		_timer						timer;
		
		friend class _guiSetupPage5;
		
		// Time Object that lasts as long as the viewswitcher exists
		_time systemTime;
		
		_callbackReturn clockEditHandler( _event );
		_callbackReturn clockCounterHandler( _event );
		_callbackReturn calendarEditHandler( _event );
		_callbackReturn drawClockImageHandler( _event );
		
		// Increases the seconds of the clock
		void increaseTime();
	
	public:
		
		//! Ctor
		_guiSetupPage3() :
			timer( make_callback( this , &_guiSetupPage3::increaseTime ) , 1000 , true )
			, systemTime( _time::now() )
		{
			this->timer.start();
		}
		
		//! Move Ctor
		_guiSetupPage3( _guiSetupPage3&& ) = default;
		
		void create( _gadget* viewParent );
		
		bool destroy( _gadget* viewParent )
		{
			_time time = this->imgClock->getTime();
			
			// Save the time on the _clockGadget
			this->systemTime.set( _timeAttr::hour , time.get( _timeAttr::hour ) );
			this->systemTime.set( _timeAttr::minute , time.get( _timeAttr::minute ) );
			this->systemTime.set( _timeAttr::second , time.get( _timeAttr::second ) );
			
			delete this->lblTitle;
			delete this->lblAdjustSystemClock;
			delete this->lblFetchTime1;
			delete this->lblFetchTime2;
			delete this->imgClock;
			delete this->calendar;
			delete this->cntSeconds;
			delete this->cntMinutes;
			delete this->cntHours;
			delete this->radManual;
			delete this->radAuto;
			delete this->radioGroup;
			delete this->colon1;
			delete this->colon2;
			
			// Switching the view is allowed
			return true;
		}
};

#endif