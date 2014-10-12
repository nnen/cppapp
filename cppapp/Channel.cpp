/**
 * \file   Channel.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2014-10-12
 * 
 * \brief  Implementation file for the Channel class.
 */

#include "Channel.h"


namespace cppapp {


int ByteChannel::drainAllInternal(ByteChannel::Callback callback)
{
	int count = 0;
	
	while (high_ > low_) {
		UByte *ptr = low_;
		
		while (ptr < high_) {
			MemBlock block;
			memcpy(&(block.length), ptr, sizeof(size_t));
			ptr += sizeof(size_t);
			block.ptr = (void*)ptr;
			callback(block);
			count++;
			ptr += block.length;
		}
		
		low_           = data_;
		high_          = secondaryHigh_;
		secondaryHigh_ = data_;
	}
	
	return count;

}


bool ByteChannel::send(void *buffer, size_t size)
{
	MutexLock lock(&mutex_);

	int totalSize = sizeof(size_t) + size;
	
	if (secondaryHigh_ > data_) {
		if ((low_ - secondaryHigh_) >= totalSize) {
			memcpy(secondaryHigh_, &size, sizeof(size));
			memcpy(secondaryHigh_ + sizeof(size_t), buffer, size);
			secondaryHigh_ += totalSize;
			return true;
		}
		return false;
	}
	
	if (((data_ + capacity_) - high_) >= totalSize) {
		memcpy(high_, &size, sizeof(size));
		memcpy(high_ + sizeof(size_t), buffer, size);
		high_ += totalSize;
		return true;
	}
	
	if ((low_ - secondaryHigh_) >= totalSize) {
		memcpy(secondaryHigh_, &size, sizeof(size));
		memcpy(secondaryHigh_ + sizeof(size_t), buffer, size);
		secondaryHigh_ += totalSize;
		return true;
	}
	
	return false;
}


bool ByteChannel::send(MemBlock buffer)
{
	return send(buffer.ptr, buffer.length);
}


int ByteChannel::drainAll(ByteChannel::Callback callback, bool blocking)
{
	MutexLock lock(&mutex_);
	
	int count = 0;
	
	if (blocking) {
		while (count < 1) {
			if (closing_) return drainAllInternal(callback);
			
			waiting_++;
			condition_.wait(mutex_);
			waiting_--;
			
			count = drainAllInternal(callback);
		}
		
		return count;
	}

	return drainAllInternal(callback);
}


} // namespace cppapp


