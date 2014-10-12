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
#ifdef CPPAPP_DEBUG
	LOG_DEBUG("Thread started...");
#endif
	Thread *t = (Thread*)arg;
	t->isRunning_ = true;
	t->isStopped_ = false;
	void *result = t->run();
#ifdef CPPAPP_DEBUG
	LOG_DEBUG("Returning from thread...");
#endif
	t->isRunning_ = false;
	t->isStopped_ = true;
	return result;
}


void Thread::exit(void *result)
{
#ifdef CPPAPP_DEBUG
	LOG_DEBUG("Exiting thread...");
#endif
	pthread_exit(result);
}


/**
 * Constructor.
 */
Thread::Thread() :
	isRunning_(false),
	isStopped_(false),
	isStopRequested_(false)
{
#ifdef CPPAPP_DEBUG
	LOG_DEBUG("Starting thread...");
#endif
	HANDLE_SYSERR(pthread_create(&thread_, NULL, threadFunction, this))
}


Thread::Thread(pthread_t thread) :
	thread_(thread),
	isRunning_(false),
	isStopped_(false),
	isStopRequested_(false)
{ }


Thread::~Thread()
{
}


void Thread::requestStop()
{
	isStopRequested_ = true;
}


void* Thread::join()
{
	void *result;
	HANDLE_SYSERR(pthread_join(thread_, &result));
	return result;
}


} // namespace cppapp


