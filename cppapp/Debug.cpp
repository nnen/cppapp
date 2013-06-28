/**
 * \file   Debug.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-06-26
 * 
 * \brief  Implementation file for the Debug class.
 */

#include "Debug.h"


namespace cppapp {


////////////////////////////////////////////////////////////////////////////////
// BACKTRACE
////////////////////////////////////////////////////////////////////////////////


void Backtrace::get(int maxSize)
{
	AddressType *buffer = new AddressType[maxSize];
		
	size_t actualSize = backtrace(buffer, maxSize);
	
	vector<AddressType> result;
	result.reserve(actualSize);
	
	for (size_t i = 0; i < actualSize; i++)
		result.push_back(buffer[i]);
	
	addresses_.swap(result);
	delete [] buffer;

	getSymbols();
}


void Backtrace::getSymbols()
{
	char **strings = backtrace_symbols(&addresses_[0], addresses_.size());
	
	std::vector<std::string> symbols;
	symbols.reserve(addresses_.size());
	
	for (unsigned int i = 0; i < addresses_.size(); i++) {
		symbols.push_back(std::string(strings[i]));
	}
	
	symbols_.swap(symbols);
	free(strings);
}


void Backtrace::print(ostream &out)
{
	for (unsigned int i = 0; i < symbols_.size(); i++) {
		out << symbols_[i] << std::endl;
	}
}


////////////////////////////////////////////////////////////////////////////////
// ASSERT
////////////////////////////////////////////////////////////////////////////////


bool Assert::throwExceptions_ = false;


void Assert::setThrow(bool value)
{
	throwExceptions_ = value;
}


void Assert::abortAssert(bool condition, const char *expr, const char *file, int line)
{
	if (condition) return;
	
	std::cerr << file << ":" << line << ": " << 
		"Assertion failed: " << expr << std::endl;
	
	abort();
}


void Assert::throwAssert(bool condition, const char *expr, const char *file, int line)
{
	if (condition) return;
	
	throw std::exception();
}


void Assert::assert_(bool condition, const char *expr, const char *file, int line)
{
	if (throwExceptions_)
		throwAssert(condition, expr, file, line);
	else
		abortAssert(condition, expr, file, line);
}


} /* namespace cppapp */

