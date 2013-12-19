#include "_type/type.time.h"

namespace std{
#include <time.h>
}

_time _time::now()
{
	return _time( std::time( NULL ) );
}

_time::_time( _int rawTime ) :
	dirty( false )
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
	timeinfo.tm_year	= this->year - 1900;
	timeinfo.tm_mon		= this->month - 1;
	timeinfo.tm_mday	= this->day;
	timeinfo.tm_hour	= this->hour;
	timeinfo.tm_min		= this->minute;
	timeinfo.tm_sec		= this->second;
	
	return std::mktime( &timeinfo ); 
}

_time::operator string()
{
	// Check this time structure is valid
	validate();
	
	std::time_t rawtime = (_int)*this;
	
	return std::ctime( &rawtime ); // Convert to UTC
}

string _time::toString( string format )
{
	// Check this time structure is valid
	validate();
	
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

_int _time::get( _timeAttr attr )
{
	// Check this time structure is valid
	validate();
	
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
				return 7;
			return this->dayOfWeek;
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

void _time::set( _timeAttr attr , _int value )
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
			return;
	}
	
	this->dirty = true;
}

void _time::validate(){
	if( dirty )
		*this = _time( _int( *this ) );
}