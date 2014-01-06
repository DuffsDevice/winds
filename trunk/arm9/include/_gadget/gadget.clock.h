// Check if already included
#ifndef _WIN_G_CLOCKGADGET_
#define _WIN_G_CLOCKGADGET_

#include "_type/type.gadget.h"
#include "_type/type.time.h"
#include "_type/type.timer.h"

class _clockGadget : public _gadget
{
	private:
		
		//! String to be displayed
		_pixel	outerBgColor;
		_pixel	innerBgColor;
		_pixel	secondsColor;
		_pixel	minutesColor;
		_pixel	hoursColor;
		_pixel	markingsColor;
		
		//! Values
		_s8		seconds;
		_s8		minutes;
		_s8		hours;
		
		//! Timer
		_timer	timer;
		
		static _callbackReturn refreshHandler( _event );
		
	public:
		
		//! Set currently displayed time
		void setIntValue( _int unixTime ){ setTime( _time(unixTime) ); }
		void setTime( _time time ){
			setTime( time.get( _timeAttr::hour ) , time.get( _timeAttr::minute ) , time.get( _timeAttr::second ) );
		}
		
		//! A more complex way
		void setTime( _u8 hour , _u8 minutes , _u8 seconds );
		
		//! Get the displayed time
		_time getTime(){ return _time::time( this->hours , this->minutes, this->seconds ); }
		
		//! Get the displayed time as a unixTime value
		_int getIntValue(){ return getTime(); }
		
		//! Set background color of the space around the clock
		void setOuterBgColor( _pixel outerBgColor ){
			if( this->outerBgColor == outerBgColor )
				return;
			this->outerBgColor = outerBgColor;
			this->redraw();
		}
		
		//! Getter for this...
		_pixel getOuterBgColor(){
			return this->outerBgColor;
		}
		
		//! Sets the background of the clock
		void setInnerBgColor( _pixel innerBgColor ){
			if( this->innerBgColor == innerBgColor )
				return;
			this->innerBgColor = innerBgColor;
			this->redraw();
		}
		
		//! Getter for this...
		_pixel getInnerBgColor(){ return this->innerBgColor; }
		
		//! Sets the color of the seconds hand
		void setSecondsColor( _pixel secondsColor ){
			if( this->secondsColor == secondsColor )
				return;
			this->secondsColor = secondsColor;
			this->redraw();
		}
		
		//! Getter for this...
		_pixel getSecondsColor(){ return this->secondsColor; }
		
		//! Sets the color of the minute hand
		void setMinutesColor( _pixel minutesColor ){
			if( this->minutesColor == minutesColor )
				return;
			this->minutesColor = minutesColor;
			this->redraw();
		}
		
		//! Getter for this...
		_pixel getMinutesColor(){ return this->minutesColor; }
		
		//! Sets the color of the hour hand
		void setHoursColor(_pixel hoursColor){
			this->hoursColor = hoursColor;
		}
		
		//! Getter for this...
		_pixel getHoursColor(){ return this->hoursColor; }
		
		//! Sets the color of the dots that mark every five minutes
		void setMarkingsColor( _pixel markingsColor ){
			if( this->markingsColor == markingsColor )
				return;
			this->markingsColor = markingsColor;
			this->redraw();
		}
		
		//! Getter for this...
		_pixel getMarkingsColor(){
			return this->markingsColor;
		}
		
		//! Set whether the clock should be increased automatically
		void setAutoIncrease( bool val ){
			if( val )
				this->timer.start();
			else
				this->timer.stop();
		}
		
		//! Chekc if automaticIncrease is active
		bool isAutoIncreasing(){ return this->timer.isRunning(); }
		
		//! Increases the clock time by one second
		void increase();
		
		//! Just for fun ;D (or at least for sake of completeness)
		void decrease();
		
		//! Complex Constructor
		_clockGadget( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _u8 hours , _u8 minutes , _u8 seconds , bool autoIncrease = true , _style&& style = _style() );
		
		//! Constructor
		_clockGadget( _optValue<_coord> x , _optValue<_coord> y , _optValue<_length> width , _optValue<_length> height , _time initialTime , bool autoIncrease = true , _style&& style = _style() )
			: _clockGadget( x , y , width , height
				, initialTime.get( _timeAttr::hour )
				, initialTime.get( _timeAttr::minute )
				, initialTime.get( _timeAttr::second )
				, autoIncrease , (_style&&)style
			)
		{}
};

#endif