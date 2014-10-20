/**
 * \file   Time.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2014-10-16
 * 
 * \brief  Implementation file for time related types.
 */

#include "Time.h"


DateTime TimeVal::toDateTime() const
{
	struct tm *dt = localtime(&value.tv_sec);
	return DateTime(dt, value.tv_usec);
}


void TimeVal::format(const char *fmt, std::ostream *out) const
{
	struct tm *ts = localtime(&value.tv_sec);
	char       buffer[STRFTIME_BUFFER_SIZE];
	
	int length = strftime(buffer, STRFTIME_BUFFER_SIZE, fmt, ts);
	if (length > 0)
		*out << buffer;
	else
		*out << "#TimeVal format error#";
}


std::ostream& operator << (std::ostream& output, const TimeVal& tm)
{
	output << tm.value.tv_sec << " sec " << tm.value.tv_usec << " usec";
	return output;
}


std::ostream& operator << (std::ostream& output, const DateTime& tm)
{
	tm.format("%Y-%m-%d %Y:%M:%S", &output);
	return output;
}


