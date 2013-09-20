#ifndef _WIN_SC_SETUP_PAGE3_
#define _WIN_SC_SETUP_PAGE3_

#include "_type/type.h"
#include "_type/type.view.h"
#include "_type/type.viewswitcher.h"
#include "_type/type.system.h"
#include "_type/type.timer.h"
#include "_type/type.gadget.h"
#include "_type/type.gadget.helpers.h"

#include "_gadget/gadget.label.h"
#include "_gadget/gadget.imagegadget.h"
#include "_gadget/gadget.counter.h"
#include "_gadget/gadget.radio.h"
#include "_gadget/gadget.calendar.h"
#include "_type/type.singlevaluegroup.h"
#include "_type/type.time.h"

class _scSetupPage3 : public _view
{
	private:
	
		_label*						lblTitle;
		_label*						lblAdjustSystemClock;
		_label*						lblFetchTime1;
		_label*						lblFetchTime2;
		_imagegadget*				imgClock;
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
		
		friend class _scSetupPage5;
		
		// Time Object that lasts as long as the viewswitcher exists
		_time systemTime;
		
		_callbackReturn clockEditHandler( _event e );
		_callbackReturn calendarEditHandler( _event e );
		_callbackReturn drawClockImageHandler( _event e );
		
		// Increases the seconds of the clock
		void increaseTime();
	
	public:
		
		//! Ctor
		_scSetupPage3() :
			timer( make_callback( this , &_scSetupPage3::increaseTime ) , 1000 , true )
			, systemTime( _time::now() )
		{
			this->timer.start();
		}
		
		//! Move Ctor
		_scSetupPage3( _scSetupPage3&& ) = default;
		
		void create( _gadget* viewParent );
		
		bool destroy( _gadget* viewParent )
		{
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