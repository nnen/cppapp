/**
 * \file   Stopwatch.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-10-03
 *
 * \brief Header file for the Stopwatch class.
 */

#ifndef STOPWATCH_PFJWTBKJ
#define STOPWATCH_PFJWTBKJ


#include <cstdlib>
#include <sys/time.h>
#include <iostream>


namespace cppapp {


#define MICROSECONDS_PER_SECOND 1000000
#define MILLISECONDS_PER_SECOND 1000
#define MICROSECONDS_PER_MILLISECOND 1000

using namespace std;

class Stopwatch {
private:
	struct timeval startTime;
	struct timeval endTime;
	
	static double timevalToMS(timeval t) {
		return ((double)t.tv_sec * (double)MILLISECONDS_PER_SECOND) +
		       ((double)t.tv_usec / (double)MICROSECONDS_PER_MILLISECOND);
	}

public:
	inline void start() {
		gettimeofday(&startTime, NULL);
	}
	
	inline void end() {
		gettimeofday(&endTime, NULL);
	}
	
	double getMilliseconds() const {
		return timevalToMS(endTime) - timevalToMS(startTime);
	}
};


ostream& operator << (ostream& output, const Stopwatch& stopwatch);


} // namespace cppapp


#endif /* end of include guard: STOPWATCH_PFJWTBKJ */

