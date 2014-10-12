/**
 * \file   Channel.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-08-19
 *
 * \brief  Header file for the Channel class.
 */

#ifndef CHANNEL_B6GPI1HN
#define CHANNEL_B6GPI1HN


#include <vector>

#include "Mutex.h"
#include "utils.h"


namespace cppapp {


/** \addtogroup threading
 * @{
 */


/**
 * \brief Thread-safe message channel (queue).
 *
 * \todo Write documentation for class Channel.
 */
template<class T>
class Channel {
private:
	std::vector<T> buffer_;
	
	Mutex          mutex_;
	Condition      condition_;
	
	int            waiting_;
	bool           closing_;
	
	/**
	 * Copy constructor.
	 */
	Channel(const Channel& other);
	
public:
	/**
	 * Constructor.
	 */
	Channel() :
		waiting_(0), closing_(false)
	{}
	
	///**
	// * Destructor.
	// */
	//virtual ~Channel() {}
	
	void send(T value)
	{
		MutexLock lock(&mutex_);
		
		buffer_.push_back(value);
		condition_.signal();
	}
	
	template<class TContainer>
	void sendAll(TContainer &cont)
	{
		if (cont.size() == 0) return;
		
		{
			MutexLock lock(&mutex_);
			
			FOR_EACH(cont, item) {
				buffer_.push_back(*item);
			}
			
			condition_.signal();
		}
	}
	
	template<class TContainer>
	bool drain(TContainer &cont)
	{
		// Lock mutex and wait for signal
		// (waiting on condition variable unlocks the mutex again).
		MutexLock lock(&mutex_);
		
		// Keep waiting on a signal until there are some
		// elements in the buffer.
		while (buffer_.empty()) {
			if (closing_) return false;
			
			waiting_++;
			condition_.wait(mutex_);
			waiting_--;
		}
		
		// Finally, copy the contents of the buffer to
		// the receiving container and return.
		cont.insert(cont.end(), buffer_.begin(), buffer_.end());
		buffer_.clear();
		return !closing_;
	}
	
	void close()
	{
		closing_ = true;
		condition_.broadcast();
	}
};


class ByteChannel {
public:
	typedef uint8_t UByte;
	typedef void (*Callback)(MemBlock item);


private:
	//MemBlock            data_;
	UByte              *data_;
	size_t              capacity_;
	
	UByte              *secondaryHigh_;
	UByte              *low_;
	UByte              *high_;
	
	Mutex               mutex_;
	Condition           condition_;

	int                 waiting_;
	bool                closing_;

	int drainAllInternal(Callback callback);


public:
	ByteChannel(size_t capacity) :
		capacity_(capacity), waiting_(0), closing_(false)
	{ }
	
	bool isClosing() const { return closing_; }
	
	bool send(void *buffer, size_t size);
	bool send(MemBlock buffer);
	
	int drainAll(Callback callback, bool blocking = true);
	
	void close();
};


/** @} */


} // namespace cppapp


#endif /* end of include guard: CHANNEL_B6GPI1HN */

