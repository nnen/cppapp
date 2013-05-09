/**
 * \file   Exception.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-05-08
 * 
 * \brief  Implementation file for the Exception class.
 */

#include "Exception.h"
#include "Logger.h"

#include <cstdlib>


namespace cppapp {


void Error::exit(const char *msg)
{
	if (!isError()) return;
	LOG_ERROR(msg << *this);
	std::exit(1);
}


void Error::exit()
{
	exit("System error occured: ");
}


ostream& operator<<(ostream &output, const Error &error)
{
	output << error.getMessage() << " (code " << error.code << ")";
	return output;
}


} // namespace cppapp


