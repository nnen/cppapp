/**
 * \file   Thread.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-05-08
 * 
 * \brief  Implementation file for the Thread class.
 */

#include "Thread.h"
#include "Exception.h"


namespace cppapp {


void* Thread::threadFunction(void *arg)
{
	Thread *t = (Thread*)arg;
	return t->run();
}


void Thread::exit(void *result)
{
	pthread_exit(result);
}


/**
 * Constructor.
 */
Thread::Thread()
{
	HANDLE_SYSERR(pthread_create(&thread_, NULL, threadFunction, this))
}


Thread::~Thread()
{
	//Error err = pthread_destroy(&thread_);
	//err.exit();
}


void* Thread::join()
{
	void *result;
	HANDLE_SYSERR(pthread_join(thread_, &result));
	return result;
}


} // namespace cppapp


