/**
 * \file   Exception.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-05-08
 *
 * \brief  Header file for the Exception class.
 */

#ifndef EXCEPTION_BHG1J3SV
#define EXCEPTION_BHG1J3SV


#include <iostream>
#include <exception>
#include <cstring>
#include <cerrno>
using namespace std;


namespace cppapp {


#define ERROR_NONE 0
#define HANDLE_SYSERR(code) { Error(code).exit(); }


struct Error {
	int code;
	
	Error(int code) : code(code) {}
	
	bool isError() const { return code != ERROR_NONE; }
	
	char* getMessage() const { return strerror(code); }
	
	void exit(const char *msg);
	void exit();
	
	operator int() const { return code; }
};


ostream& operator<<(ostream &output, const Error &error);


/**
 * \todo Write documentation for class Exception.
 */
class Exception : public std::exception {
private:
	/**
	 * Copy constructor.
	 */
	Exception(const Exception& other);
	
public:
	/**
	 * Constructor.
	 */
	Exception();
};


} // namespace cppapp


#endif /* end of include guard: EXCEPTION_BHG1J3SV */

