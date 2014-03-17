/**
 * \file   Stopwatch.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-10-03
 * 
 * \brief Implementation file for the Stopwatch class.
 */

#include "Stopwatch.h"


namespace cppapp {


ostream& operator << (ostream& output, const Stopwatch& stopwatch)
{
	output << stopwatch.getMilliseconds();
	return output;
}


} // namespace cppapp

