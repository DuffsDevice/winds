#ifndef _WIN_T_TIME_
#define _WIN_T_TIME_

#include "_type/type.h"

enum class _timeAttr : _u8
{
	year,
	month,
	day,
	dayOfWeek,
	hour,
	minute,
	second
};

extern _fromStr<_timeAttr> string2timeAttr;
extern _toStr<_timeAttr> timeAttr2string;

class _time
{
	private:
	
		_int second;
		_int minute;
		_int hour;
		_int day;
		_int dayOfWeek;
		_int month;
		_int year;
		bool dirty; // Indicates that the given attributes are not validated
		
		// Validates this _time structure if it's necessary
		void validate();
		
	public:
	
		//! Get a time struct of >>now<<
		static _time now();
		
		//! Generate a _time struct of the supplied date (time will be 00:00:00)
		static _time date( _int year , _int month , _int day )
		{
			_time t;
			t.year = year;
			t.month = month;
			t.day = day;
			t.dirty = true;
			return t;
		}
		
		//! Generate a _time struct of the supplied time
		static _time time( _int hour , _int minute , _int second )
		{
			_time t;
			t.hour = hour;
			t.minute = minute;
			t.second = second;
			t.dirty = true;
			return t;
		}
		
		//! Ctor
		_time() :
			second( 0 ) , minute( 0 )
			, hour( 0 ) , day( 0 )
			, dayOfWeek( 0 ) , month( 0 )
			, year( 1970 )
			, dirty( false )
		{}
		
		//! Ctor using UNIX Time
		_time( _unixTime rawTime );
		
		//! Convert the _time structure to UNIX time
		inline operator _unixTime() const {
			return this->toUnixTime();
		}
		
		//! Convert the _time to string
		inline operator string(){
			return this->toString( ignore );
		}
		
		//! Adds the specified (signed) amount of time to the given _time field, based on the calendar's rules
		void add( _timeAttr attr , _int diff ){ this->set( attr , this->get( attr ) + diff ); }
		
		//! Returns the given _time field
		_unixTime get( _timeAttr attr );
		
		//! Applies the specified amount of time to the given _time field
		//! @note you cannot set the weekday!
		void set( _timeAttr attr , _int value );
		
		//! Convert the _time to string using a specific format
		//! @see http://www.cplusplus.com/reference/ctime/strftime/#parameters
		string toString( _optValue<_literal> format = ignore );
		
		//! Convert this time struct to a number (Unix-Time)
		_unixTime toUnixTime() const ;
};

#endif