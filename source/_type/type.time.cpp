#include "_type/type.time.h"

#include <time.h>

_time _time::now()
{
	return _time( time( NULL ) );
}

_time::_time( _u32 rawTime )
{
	time_t rawtime = rawTime;
	
	struct tm * timeinfo = localtime( &rawtime ); // Convert to UTC
	
	this->month = timeinfo->tm_mon + 1;
	this->year = timeinfo->tm_year + 1900;
	this->day = timeinfo->tm_mday;
	this->hour = timeinfo->tm_hour;
	this->minute = timeinfo->tm_min;
	this->second = timeinfo->tm_sec;
	this->weekday = timeinfo->tm_wday;
}

_time::operator _u32()
{
	struct tm timeinfo;
	timeinfo.tm_year = year - 1900;
	timeinfo.tm_mon = month - 1;
	timeinfo.tm_mday = day;
	timeinfo.tm_hour  = hour;
	timeinfo.tm_min   = minute;
	timeinfo.tm_sec   = second;
	
	return mktime( &timeinfo ); 
}

_time::operator string()
{
	time_t rawtime;
	
	time( &rawtime ); // Read Time
	
	return ctime( &rawtime ); // Convert to UTC
}