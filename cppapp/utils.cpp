/**
 * \file   utils.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-03-16
 *
 * \brief  Implementation file for the utilities declared in utils.h
 */

#include "utils.h"


namespace cppapp {


int bitLength(long value)
{
	int bitLength = 1;
	
	for (unsigned int bit = 0; bit < (sizeof(long) * CHAR_BIT); bit++) {
		if (value & 1) bitLength = (bit + 1);
		value >>= 1;
	}
	
	return bitLength;
}


uint32_t networkToHost(uint32_t value)
{
	if (isBigEndian()) return value;
	
	union {
		char raw[sizeof(uint32_t)];
		uint32_t value;
	} newValue;
	
	newValue.value = value;
	#define SWAP(a, b) { char c = (a); (a) = (b); (b) = c; }
	SWAP(newValue.raw[0], newValue.raw[3])
	SWAP(newValue.raw[1], newValue.raw[2])
	#undef SWAP
	return newValue.value;
}


uint32_t hostToNetwork(uint32_t value)
{
	return networkToHost(value);
}


uint32_t hostToLittleEndian(uint32_t value)
{
	if (!isBigEndian()) return value;
	
	union {
		char     raw[sizeof(uint32_t)];
		uint32_t value;
	} newValue;
	
	newValue.value = value;
	#define SWAP(a, b) { char c = (a); (a) = (b); (b) = c; }
	SWAP(newValue.raw[0], newValue.raw[3])
	SWAP(newValue.raw[1], newValue.raw[2])
	#undef SWAP
	return newValue.value;
}


uint32_t littleEndianToHost(uint32_t value)
{
	return hostToLittleEndian(value);
}


int ceilDiv(int a, int b)
{
	return ((a / b) + ((a % b) ? 1 : 0));
}


} // namespace cppapp

