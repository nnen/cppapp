/**
 * \file   Mutex.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-05-08
 * 
 * \brief  Implementation file for the Mutex class.
 */

#include "Mutex.h"


namespace cppapp {


/**
 * Constructor.
 */
MutexLock::MutexLock(Mutex *mutex) :
	mutex_(mutex)
{
	if (mutex_ != NULL)
		mutex_->lock();
}


MutexLock::~MutexLock()
{
	if (mutex_ != NULL)
		mutex_->unlock();
	mutex_ = NULL;
}


} /* namespace cppapp */

