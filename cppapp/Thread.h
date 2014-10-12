/**
 * \file   Thread.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-05-08
 *
 * \brief  Header file for the Thread class.
 */

#ifndef THREAD_K9TW92HX
#define THREAD_K9TW92HX


#include <cassert>
#include <pthread.h>


namespace cppapp {


/** \addtogroup threading
 *
 * \brief POSIX thread library wrapper
 *
 * @{
 */


/**
 * \todo Write documentation for class Thread.
 */
class Thread {
private:
	/**
	 * Copy constructor.
	 */
	Thread(const Thread& other);
	
	Thread(pthread_t t);
	
	pthread_t thread_;
	
	static void* threadFunction(void *arg);
	
	bool isRunning_;
	bool isStopped_;
	bool isStopRequested_;

protected:
	void exit(void *result);

public:
	/**
	 * Constructor.
	 */
	Thread();
	/**
	 * Destructor.
	 */
	virtual ~Thread();

	virtual void requestStop();
	
	void* join();
	
	virtual void* run() = 0;
	
	bool isRunning() { return isRunning_; }
	bool isStopRequested() { return isStopRequested_; }
};


/**
 * \todo Write documentation for class MethodThread : public Thread.
 */
template<class TReturn, class TClass>
class MethodThread : public Thread {
public:
	typedef TReturn* (TClass::*Method)();

private:
	TClass  *instance_;
	Method   method_;
	TReturn *returnValue_;
	
public:
	MethodThread(TClass *instance, Method method) :
		Thread(),
		instance_(instance),
		method_(method)
	{
		assert(instance != NULL);
		assert(method != NULL);
	}
	
	virtual ~MethodThread() {}
	
	virtual void* run()
	{
		returnValue_ = (instance_->*method_)();
		return returnValue_;
	}
	
	TReturn* returnValue() const { return returnValue_; }
};


/** @} */


} // namespace cppapp


#endif /* end of include guard: THREAD_K9TW92HX */

