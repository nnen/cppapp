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


/**
 * \brief Represents a unix system error code.
 */
struct Error {
	/// The value system error code.
	int code;
	
	/**
	 * \brief Constructor.
	 *
	 * \param code the system error code
	 */
	Error(int code) : code(code) {}
	
	/**
	 * \brief Return \c true if the error code is an actual error, return
	 *        false otherwise.
	 */
	bool isError() const { return code != ERROR_NONE; }
	
	/**
	 * \brief Return the error code message as returned by the \c strerror
	 *        stdlib function.
	 */
	char* getMessage() const { return strerror(code); }
	
	/**
	 * \brief If the code is an error, logs the error and exits the program.
	 *
	 * \param msg A message to be logged before the system error code and message.
	 */
	void exit(const char *msg);
	/**
	 * \brief If the code is an error, logs the error and exits the program.
	 */
	void exit();
	
	/**
	 * \brief Convert the Error instance to its error code value.
	 */
	operator int() const { return code; }
};


ostream& operator<<(ostream &output, const Error &error);


/**
 * \brief Base class for cppapp/user exceptions.
 */
class Exception : public std::exception {
private:
	/**
	 * \brief Copy constructor.
	 */
	Exception(const Exception& other);
	
public:
	/**
	 * \brief Constructor.
	 */
	Exception();
};


} // namespace cppapp


#endif /* end of include guard: EXCEPTION_BHG1J3SV */

