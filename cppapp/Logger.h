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
		cppapp::Logger::error() << cppapp::Logger::Entry(__FILE__, __LINE__) << \
		message__ << flush << endl; }
#	define LOG_WARNING(message__) { \
		cppapp::Logger::warning() << cppapp::Logger::Entry(__FILE__, __LINE__) << \
		message__ << flush << endl; }
#	define LOG_INFO(message__) { \
		cppapp::Logger::info() << cppapp::Logger::Entry(__FILE__, __LINE__) << \
		message__ << flush << endl; }
#	ifndef NDEBUG
#		define LOG_DEBUG(message__) { \
			cppapp::Logger::debug() << cppapp::Logger::Entry(__FILE__, __LINE__) << \
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
#include "Time.h"

using namespace std;


namespace cppapp {


// This symbol is defined in "DynObject.h".
// Cannot simply include because of circular dependencies.
class DynObject;


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


class LoggerListener;


/**
 * \brief Simple logging class.
 */
class Logger : public Object {
private:
	static vector<Logger> levels_;
	
	LogLevel             level_;
	vector<Ref<Output> > outputs_;
	
	vector<Ref<LoggerListener> > listeners_;
	
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
	
	void log(const char *fileName, int lineNumber, const string &message);
	
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
	
	static void readConfig(Ref<DynObject> config);
	
	static vector<string> getBacktrace();
	
	static LogLevel logLevelFromString(std::string value);
	
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
	const char *format;

	LogTime() :
		format("%Y-%m-%d %H:%M:%S")
	{ }
	
	LogTime(const char *format) :
		format(format)
	{ }
};

ostream& operator << (ostream& output, const LogTime& tm);


class LoggerListener : public Object {
public:
	virtual void handleMessage(LogLevel      level,
				            TimeVal       time,
				            const string &fileName,
				            int           lineNumber,
				            const string &message) = 0;
};


class OutputLoggerListener : public Object {
private:
	Ref<Output> output_;

public:
	virtual void handleMessage(LogLevel      level,
				            TimeVal       time,
				            const string &fileName,
				            int           lineNumber,
				            const string &message)
	{
		*(output_->getStream()) << time << " " << level << " " << message << endl;
	}
};


} // namespace cppapp


#endif /* end of include guard: LOGGER_7CJ4HP4Z */

