/**
 * \file   Logger.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-11-02
 *
 * \brief Header file for the Logger class.
 */

#ifndef LOGGER_7CJ4HP4Z
#define LOGGER_7CJ4HP4Z


#ifdef LOG_DISABLE
#	define LOG_ERROR(message__)
#	define LOG_WARNING(message__)
#	define LOG_INFO(message__)
#	define LOG_DEBUG(message__)
#	define LOG_ASSERTION(expr)
#	define LOG_EXPR(expr)
#else // LOG_DISABLE
#	define LOG_ERROR(message__) { \
		Logger::error() << Logger::Entry(__FILE__, __LINE__) << \
		message__ << flush << endl; }
#	define LOG_WARNING(message__) { \
		Logger::warning() << Logger::Entry(__FILE__, __LINE__) << \
		message__ << flush << endl; }
#	define LOG_INFO(message__) { \
		Logger::info() << Logger::Entry(__FILE__, __LINE__) << \
		message__ << flush << endl; }
#	ifndef NDEBUG
#		define LOG_DEBUG(message__) { \
			Logger::debug() << Logger::Entry(__FILE__, __LINE__) << \
			message__ << flush << endl; }
#		define LOG_ASSERTION(expr) { \
			if (!(expr)) LOG_ERROR("Assertion \"" #expr "\" failed!"); \
			assert(expr); }
#	else
#		define LOG_DEBUG(message__)
#		define LOG_ASSERTION(expr)
#	endif
#	define LOG_EXPR(expr) LOG_DEBUG(#expr " = " << (expr))
#endif // LOG_DISABLE


#include <vector>
#include <string>
#include <sstream>
#include <ctime>

#include "utils.h"
#include "Object.h"
#include "Output.h"

using namespace std;


namespace cppapp {


/**
 * \brief Enumeration of the logging levels used by Logger. 
 */
enum LogLevel {
	LOG_LVL_NONE,
	LOG_LVL_ERROR,
	LOG_LVL_WARNING,
	LOG_LVL_INFO,
	LOG_LVL_DEBUG
};


/**
 * \brief Simple logging class.
 */
class Logger : public Object {
private:
	static vector<Logger> levels_;
	
	LogLevel             level_;
	vector<Ref<Output> > outputs_;
	
public:
	STATIC_CTOR_HEADER(Logger)
	
	Logger() : level_(LOG_LVL_NONE) { }
	Logger(LogLevel level) : level_(level) { }
	
	struct Entry;
	Logger& operator << (Logger::Entry entry);
	
	template<class T>
	Logger& operator << (T value)
	{
		FOR_EACH(outputs_, output) {
			*((*output)->getStream()) << value;
		}
		
		return *this;
	}
	
	typedef ostream& (*OStreamManipulator)(ostream& os);
	Logger& operator << (OStreamManipulator manip)
	{
		FOR_EACH(outputs_, output)
			*((*output)->getStream()) << manip;

		return *this;
	}
	
	Logger& operator << (const vector<string>& strings)
	{
		FOR_EACH(strings, s) {
			*this << *s << endl;
		}
		return *this;
	}
	
	Logger& addOutput(Ref<Output> output) { outputs_.push_back(output); return *this; }
	Logger& clearOutputs() { outputs_.clear(); return *this; }
	
	static const char* logLevelToString(LogLevel level);
	
	static Logger& getLogger(LogLevel level);
	
	static Logger& error()   { return getLogger(LOG_LVL_ERROR); }
	static Logger& warning() { return getLogger(LOG_LVL_WARNING); }
	static Logger& info()    { return getLogger(LOG_LVL_INFO); }
	static Logger& debug()   { return getLogger(LOG_LVL_DEBUG); }
	
	static void addOutput(LogLevel level, Ref<Output> output);
	static void addOutput(LogLevel level, string fileName);
	static void clearConfig();
	
	static void defaultConfig(string fileName);
	static void defaultConfig();
	
	static vector<string> getBacktrace();
	
	
	struct Entry {
		string fileName;
		int    lineNumber;
		
		Entry() :
			fileName("<unknown>"),
			lineNumber(0)
		{}
		
		Entry(string fileName, int lineNumber) :
			fileName(fileName),
			lineNumber(lineNumber)
		{}
	};
};


struct LogTime {
};

ostream& operator << (ostream& output, const LogTime& tm);


} // namespace cppapp


#endif /* end of include guard: LOGGER_7CJ4HP4Z */

