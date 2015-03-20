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
	
		_uniquePtr<_label>						lblTitle;
		_uniquePtr<_label>						lblAdjustSystemClock;
		_uniquePtr<_label>						lblFetchTime1;
		_uniquePtr<_label>						lblFetchTime2;
		_uniquePtr<_clockGadget>				imgClock;
		_uniquePtr<_calendar>					calendar;
		_uniquePtr<_counter>					cntSeconds;
		_uniquePtr<_counter>					cntMinutes;
		_uniquePtr<_counter>					cntHours;
		_uniquePtr<_radio>						radManual;
		_uniquePtr<_radio>						radAuto;
		_uniquePtr<_singleValueGroup<_radio>>	radioGroup;
		_uniquePtr<_label>						colon1;
		_uniquePtr<_label>						colon2;
		_timer									timer;
		
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
		
		//! Copy Ctor removed
		_guiSetupPage3( const _guiSetupPage3& ) = delete;
		
		//! Move Ctor
		_guiSetupPage3( _guiSetupPage3&& other ){
			this->lblTitle = move(other.lblTitle);
			this->lblAdjustSystemClock = move(other.lblAdjustSystemClock);
			this->lblFetchTime1 = move(other.lblFetchTime1);
			this->lblFetchTime2 = move(other.lblFetchTime2);
			this->imgClock = move(other.imgClock);
			this->calendar = move(other.calendar);
			this->cntSeconds = move(other.cntSeconds);
			this->cntMinutes = move(other.cntMinutes);
			this->cntHours = move(other.cntHours);
			this->radManual = move(other.radManual);
			this->radAuto = move(other.radAuto);
			this->radioGroup = move(other.radioGroup);
			this->colon1 = move(other.colon1);
			this->colon2 = move(other.colon2);
			this->timer = move(other.timer);
			this->timer.setCallback( make_callback( this , &_guiSetupPage3::increaseTime ) );
		}
		
		void create( _gadget* viewParent );
		
		bool destroy( _gadget* viewParent )
		{
			_time time = this->imgClock->getTime();
			
			// Save the time on the _clockGadget
			this->systemTime.set( _timeAttr::hour , time.get( _timeAttr::hour ) );
			this->systemTime.set( _timeAttr::minute , time.get( _timeAttr::minute ) );
			this->systemTime.set( _timeAttr::second , time.get( _timeAttr::second ) );
			
			this->lblTitle = nullptr;
			this->lblAdjustSystemClock = nullptr;
			this->lblFetchTime1 = nullptr;
			this->lblFetchTime2 = nullptr;
			this->imgClock = nullptr;
			this->calendar = nullptr;
			this->cntSeconds = nullptr;
			this->cntMinutes = nullptr;
			this->cntHours = nullptr;
			this->radManual = nullptr;
			this->radAuto = nullptr;
			this->radioGroup = nullptr;
			this->colon1 = nullptr;
			this->colon2 = nullptr;
			
			// Switching the view is allowed
			return true;
		}
};

#endif