/**
 * \file   Time.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2014-10-16
 *
 * \brief  Header file for time related types.
 */

#ifndef TIME_5HKJ7NHZ
#define TIME_5HKJ7NHZ


#include <iostream>
#include <cstring>
#include <ctime>
#include <sys/time.h>


#define STRFTIME_BUFFER_SIZE 80


struct DateTime;


struct TimeVal {
	struct timeval value;
	
	TimeVal()
	{
		value.tv_usec = 0;
		value.tv_sec = 0;
	}
	
	TimeVal(struct timeval value)
		: value(value)
	{}
	
	inline void now()
	{
		gettimeofday(&value, NULL);
	}
	
	DateTime toDateTime() const;
	
	void format(const char *fmt, std::ostream *out) const;
};

std::ostream& operator << (std::ostream& output, const TimeVal& tm);


/**
 *
 * int    tm_sec   seconds [0,61]
 * int    tm_min   minutes [0,59]
 * int    tm_hour  hour [0,23]
 * int    tm_mday  day of month [1,31]
 * int    tm_mon   month of year [0,11]
 * int    tm_year  years since 1900
 * int    tm_wday  day of week [0,6] (Sunday = 0)
 * int    tm_yday  day of year [0,365]
 * int    tm_isdst daylight savings flag
 */
struct DateTime {
	suseconds_t microseconds;
	
	int         seconds;
	int         minutes;
	int         hour;
	int         day;
	int         month;
	int         year;
	int         weekday;
	int         dayOfYear;
	int         daylightSavings;
	
	DateTime()
	{
		memset((char*)this, 0, sizeof(DateTime));
	}
	
	DateTime(struct tm *value)
	{
		set(value);
	}

	DateTime(struct tm *value, suseconds_t microseconds)
	{
		set(value, microseconds);
	}
	
	DateTime(struct tm value)
	{
		set(&value);
	}
	
	DateTime(struct tm value, suseconds_t microseconds)
	{
		set(&value, microseconds);
	}
	
	DateTime(TimeVal timeVal)
	{
		*this = timeVal.toDateTime();
	}
	
	inline void set(struct tm *value, suseconds_t microseconds)
	{
		this->microseconds = microseconds;
		
		seconds         = value->tm_sec;
		minutes         = value->tm_min;
		hour            = value->tm_hour;
		day             = value->tm_mday;
		month           = value->tm_mon;
		year            = value->tm_year;
		weekday         = value->tm_wday;
		dayOfYear       = value->tm_yday;
		daylightSavings = value->tm_isdst;
	}
	
	inline void set(struct tm *value) { set(value, 0); }

	inline void format(const char *fmt, std::ostream *out) const
	{
		//struct tm *ts = localtime(&value.tv_sec);
		char       buffer[STRFTIME_BUFFER_SIZE];
		
		int length = strftime(buffer, STRFTIME_BUFFER_SIZE, fmt, (struct tm*)&seconds);
		if (length > 0)
			*out << buffer;
		else
			*out << "#TimeVal format error#";
	}
};

std::ostream& operator << (std::ostream& output, const DateTime& tm);


#endif /* end of include guard: TIME_5HKJ7NHZ */

