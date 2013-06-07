#ifndef _WIN_T_TIME
#define _WIN_T_TIME

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
			return _time( _int( t ) );
		}
		
		//! Generate a _time struct of the supplied time (date will be 1. Jan. 1900)
		static _time time( _int hour , _int minute , _int second )
		{
			_time t;
			t.hour = hour;
			t.minute = minute;
			t.second = second;
			return _time( _int( t ) );
		}
		
		//! Ctor
		_time() :
			second( 0 ) , minute( 0 )
			, hour( 0 ) , day( 0 )
			, dayOfWeek( 0 ) , month( 0 )
			, year( 0 )
		{}
		
		//! Ctor with UNIX Time
		_time( _int );
		
		//! Convert the _time structure to UNIX time
		operator _int() const ;
		
		//! Convert the _time to string
		operator string() const ;
		
		//! Adds the specified (signed) amount of time to the given _time field, based on the calendar's rules
		void add( _timeAttr attr , _int diff , bool validate = true ){ this->set( attr , this->get( attr ) + diff , validate ); }
		
		//! Returns the given _time field
		_int get( _timeAttr attr ) const ;
		
		//! Applies the specified amount of time to the given _time field
		//! @note you cannot set the weekday!
		void set( _timeAttr attr , _int value , bool validate = true );
		
		//! Convert the _time to string using a specific format
		//! @see http://www.cplusplus.com/reference/ctime/strftime/#parameters
		string toString( string format ) const ;
};

#endif