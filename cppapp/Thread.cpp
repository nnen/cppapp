/**
 * \file   Thread.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-05-08
 * 
 * \brief  Implementation file for the Thread class.
 */

#include "Thread.h"
#include "Exception.h"
#include "Logger.h"


namespace cppapp {


void* Thread::threadFunction(void *arg)
{
	LOG_DEBUG("Thread started...");
	Thread *t = (Thread*)arg;
	void *result = t->run();
	LOG_DEBUG("Returning from thread...");
	return result;
}


void Thread::exit(void *result)
{
	LOG_DEBUG("Exiting thread...");
	pthread_exit(result);
}


/**
 * Constructor.
 */
Thread::Thread()
{
	LOG_DEBUG("Starting thread...");
	HANDLE_SYSERR(pthread_create(&thread_, NULL, threadFunction, this))
}


Thread::~Thread()
{
}


void* Thread::join()
{
	void *result;
	HANDLE_SYSERR(pthread_join(thread_, &result));
	return result;
}


} // namespace cppapp


