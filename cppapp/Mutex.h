/**
 * \file   Mutex.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-05-08
 *
 * \brief  Header file for the Mutex class.
 */

#ifndef MUTEX_6QO73ENJ
#define MUTEX_6QO73ENJ

#include "Object.h"
#include "Exception.h"
#include "Logger.h"

#include <pthread.h>


namespace cppapp {


/** \addtogroup threading
 * @{
 */


class Mutex;
class Condition;


/**
 * \todo Write documentation for class MutexAttributes
 */
class MutexAttributes {
friend class Mutex;
private:
	pthread_mutexattr_t attr_;

public:
	MutexAttributes()
	{
		HANDLE_SYSERR(pthread_mutexattr_init(&attr_));
	}
	
	~MutexAttributes()
	{
		HANDLE_SYSERR(pthread_mutexattr_destroy(&attr_));
	}
};


struct MutexLock {
private:
	MutexLock(const MutexLock& other);
	
	Mutex *mutex_;

public:
	MutexLock(Mutex *mutex);
	~MutexLock();
};


/**
 * \todo Write documentation for class Mutex.
 */
class Mutex {
friend class Condition;
private:
	/**
	 * Copy constructor.
	 */
	Mutex(const Mutex &other);
	
	pthread_mutex_t mutex_;
	
public:
	/**
	 * Constructor.
	 */
	Mutex(const MutexAttributes &attr)
	{
		HANDLE_SYSERR(pthread_mutex_init(&mutex_, &(attr.attr_)));
	}
	/**
	 * Constructor. Uses default attributes.
	 */
	Mutex()
	{
		HANDLE_SYSERR(pthread_mutex_init(&mutex_, NULL));
	}
	/**
	 * Destructor.
	 */
	~Mutex()
	{
		HANDLE_SYSERR(pthread_mutex_destroy(&mutex_));
	}
	
	void lock()
	{
		LOG_DEBUG("Locking mutex...");
		HANDLE_SYSERR(pthread_mutex_lock(&mutex_));
	}
	
	void unlock()
	{
		HANDLE_SYSERR(pthread_mutex_unlock(&mutex_));
	}
	
	bool tryLock()
	{
		Error err = pthread_mutex_trylock(&mutex_);
		if (err == EBUSY) return false;
		err.exit();
		return true;
	}
};


/**
 * \todo Write documentation for class Condition.
 */
class Condition {
private:
	Condition(const Condition &other);
	
	pthread_cond_t cond_;
	
public:
	/**
	 * Constructor.
	 */
	Condition()
	{
		HANDLE_SYSERR(pthread_cond_init(&cond_, NULL));
	}
	
	/**
	 * Destructor.
	 */
	~Condition()
	{
		HANDLE_SYSERR(pthread_cond_destroy(&cond_));
	}
	
	void wait(Mutex &mutex)
	{
		HANDLE_SYSERR(pthread_cond_wait(&cond_, &mutex.mutex_));
	}
	
	void signal()
	{
		HANDLE_SYSERR(pthread_cond_signal(&cond_));
	}

	void broadcast()
	{
		HANDLE_SYSERR(pthread_cond_broadcast(&cond_));
	}
};


/** @} */


} /* namespace cppapp */


#endif /* end of include guard: MUTEX_6QO73ENJ */

