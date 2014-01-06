/**
 * \file   Debug.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-06-26
 *
 * \brief  Header file for the Debug class.
 */

#ifndef DEBUG_UDCRJS1F
#define DEBUG_UDCRJS1F


#include <execinfo.h>
#include <cstdlib>
#include <cstdio>

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cxxabi.h>

#include "utils.h"


namespace cppapp {


////////////////////////////////////////////////////////////////////////////////
// BACKTRACE
////////////////////////////////////////////////////////////////////////////////


#define CPPAPP_BACKTRACE_LIMIT 256


class Backtrace {
public:
	typedef void* AddressType;

private:
	std::vector<AddressType> addresses_;
	std::vector<std::string> symbols_;
	std::vector<std::string> lines_;
	
	bool getLines()
	{
		std::stringstream tmpStream;
		std::vector<std::string> lines;
		
		FOR_EACH(addresses_, addr) {
			tmpStream.str("");
			tmpStream << "addr2line -fCp " << *addr << " 2> /dev/null";
			
			FILE *pipe = popen(tmpStream.str().c_str(), "r");
			if (!pipe) {
				tmpStream.str("");
				tmpStream << "[" << *addr << "]";
				lines.push_back(tmpStream.str());
				continue;
			}
			
			char buffer[128];
			tmpStream.str("");
			while (!feof(pipe)) {
				if (fgets(buffer, 128, pipe) != NULL) {
					for (char *p = buffer; (p < buffer + 128) && *p != '\n'; p++)
						tmpStream << *p;
				}
			}
			pclose(pipe);
			
			lines.push_back(tmpStream.str());
		}
		
		lines_.swap(lines);
		return true;
	}
	
public:
	typedef std::vector<std::string>::const_iterator Iterator;
	
	int size() const { return (int)addresses_.size(); }
	
	Iterator begin() const { return lines_.begin(); }
	Iterator end() const { return lines_.end(); }
	
	std::string operator[](int index) { return lines_[index]; }
	
	void get(int maxSize = CPPAPP_BACKTRACE_LIMIT);
	void getSymbols();
	
	void print(ostream &out) const;
	
	static void print();
};


std::ostream& operator<< (std::ostream &output, const Backtrace &bt);


////////////////////////////////////////////////////////////////////////////////
// ASSERT
////////////////////////////////////////////////////////////////////////////////


#ifdef NDEBUG
#	define CPPAPP_ASSERT(expr)
#else
/**
 * \brief Evaluates the expression \c expr and if it is false, either aborts
 *        the program or throws an exception.
 * 
 * The action taken can be changed using Assert methods. For more information,
 * see cppapp::Assert::assert_() and cppapp::Assert::setThrow().
 *
 * Like the original \ref assert macro, this macro is disabled if the
 * \ref NDEBUG macro is defined.
 *
 * \see cppapp::Assert::assert_()
 * \see cppapp::Assert::setThrow()
 */
#	define CPPAPP_ASSERT(expr) \
		{ cppapp::Assert::assert_((expr), #expr, __FILE__, __LINE__); }
#endif


/**
 * \brief Works like the CPPAPP_ASSERT(expr) macro, except it always throws an
 *        exception regardless of the Assert class state.
 */
#define CPPAPP_THROW_ASSERT(expr) \
	{ cppapp::Assert::throwAssert((expr), #expr, __FILE__, __LINE__); }


/**
 * \brief Static class replacing the standard \c assert macro in \c cassert.
 *
 * Preferred usage of this class is through the CPPAPP_ASSERT(expr) and
 * CPPAPP_THROW_ASSERT(expr) macros.
 *
 * For more information on the \c assert macro, see  
 * http://www.cplusplus.com/reference/cassert/ .
 *
 * \see CPPAPP_ASSERT
 * \see CPPAPP_THROW_ASSERT
 */
class Assert {
private:
	static bool throwExceptions_;
	
	Assert();

public:
	/**
	 * \brief Sets whether CPPAPP_ASSERT should throw an exception (if
	 *        \c true) or abort the program.
	 *
	 * \see CPPAPP_ASSERT
	 * \see Assert::assert_
	 */
	static void setThrow(bool value);
	
	static void abortAssert(bool condition,
					    const char *expr,
					    const char *file,
					    int line);
	static void throwAssert(bool condition,
					    const char *expr,
					    const char *file,
					    int line);
	
	/**
	 * \brief The main interface of the Assert class.
	 *
	 * Based on internal logic, this method simply calls Assert::abortAssert()
	 * or Assert::throwAssert().  To controll this behavior, call
	 * Assert::setThrow().
	 *
	 * \param condition The condition that is expected to be \c true.
	 * \param expr      The expression whose value is passed to \c condition.
	 * \param file      The name of the file from which the assertion has been called.
	 * \param line      Line in the file where the assertion is located.
	 *
	 * \see CPPAPP_ASSERT
	 * \see CPPAPP_THROW_ASSERT
	 * \see Assert::setThrow
	 */
	static void assert_(bool condition, const char *expr, const char *file, int line);
};


} /* namespace cppapp */


#endif /* end of include guard: DEBUG_UDCRJS1F */


