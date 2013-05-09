/**
 * \file   Thread.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-05-08
 *
 * \brief  Header file for the Thread class.
 */

#ifndef THREAD_K9TW92HX
#define THREAD_K9TW92HX


#include <pthread.h>


namespace cppapp {


/**
 * \todo Write documentation for class Thread.
 */
class Thread {
private:
	/**
	 * Copy constructor.
	 */
	Thread(const Thread& other);
	
	pthread_t thread_;
	
	static void* threadFunction(void *arg);

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
	~Thread();

	void* join();
	
	virtual void* run() = 0;
};


} // namespace cppapp


#endif /* end of include guard: THREAD_K9TW92HX */

