#include "_type/type.time.h"

namespace std{
#include <time.h>
}

_time _time::now()
{
	return _time( std::time( NULL ) );
}

_time::_time( _int rawTime )
{
	std::time_t rawtime = rawTime;
	
	struct std::tm * timeinfo = std::gmtime( &rawtime ); // Convert to UTC
	
	this->month = timeinfo->tm_mon + 1;
	this->year = timeinfo->tm_year + 1900;
	this->day = timeinfo->tm_mday;
	this->hour = timeinfo->tm_hour;
	this->minute = timeinfo->tm_min;
	this->second = timeinfo->tm_sec;
	this->dayOfWeek = timeinfo->tm_wday;
}

_time::operator _int() const 
{
	struct std::tm timeinfo;
	timeinfo.tm_year	= year - 1900;
	timeinfo.tm_mon		= month - 1;
	timeinfo.tm_mday	= day;
	timeinfo.tm_hour	= hour;
	timeinfo.tm_min		= minute;
	timeinfo.tm_sec		= second;
	
	return std::mktime( &timeinfo ); 
}

_time::operator string() const 
{
	std::time_t rawtime = (_int)*this;
	
	return std::ctime( &rawtime ); // Convert to UTC
}

string _time::toString( string format ) const 
{
	_char str[127];
	
	struct std::tm timeinfo;
	timeinfo.tm_year	= year - 1900;
	timeinfo.tm_mon		= month - 1;
	timeinfo.tm_mday	= day;
	timeinfo.tm_hour	= hour;
	timeinfo.tm_min		= minute;
	timeinfo.tm_sec		= second;
	
	strftime( str , 127 , format.c_str() , &timeinfo );
	
	return string(str);
}

_int _time::get( _timeAttr attr ) const 
{
	switch( attr )
	{
		case _timeAttr::year:
			return this->year;
		case _timeAttr::month:
			return this->month;
		case _timeAttr::day:
			return this->day;
		case _timeAttr::dayOfWeek:
			if( !this->dayOfWeek )
				return 6;
			return this->dayOfWeek - 1;
		case _timeAttr::hour:
			return this->hour;
		case _timeAttr::minute:
			return this->minute;
		case _timeAttr::second:
			return this->second;
		default:
			break;
	}
	
	return 0;
}

void _time::set( _timeAttr attr , _int value , bool validate )
{
	switch( attr )
	{
		case _timeAttr::year:
			this->year = value;
			break;
		case _timeAttr::month:
			this->month = value;
			break;
		case _timeAttr::day:
		case _timeAttr::dayOfWeek:
			this->day = value;
			break;
		case _timeAttr::hour:
			this->hour = value;
			break;
		case _timeAttr::minute:
			this->minute = value;
			break;
		case _timeAttr::second:
			this->second = value;
			break;
		default:
			break;
	}
	
	// Validate this _time structure
	if( validate )
		*this = _time( _int( *this ) );
}