/**
 * \file   Logger.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-11-02
 * 
 * \brief Implementation file for the Logger class.
 */


#include <execinfo.h>
#include <cstdlib>

#include "Logger.h"


namespace cppapp {


vector<Logger> Logger::levels_;


STATIC_CTOR_IMPL(Logger)
{
	levels_.push_back(Logger(LOG_LVL_NONE));
	levels_.push_back(Logger(LOG_LVL_ERROR));
	levels_.push_back(Logger(LOG_LVL_WARNING));
	levels_.push_back(Logger(LOG_LVL_DEBUG));

	defaultConfig();
}


Logger& Logger::operator << (Logger::Entry entry)
{
	(*this)
		<< "[" << Logger::logLevelToString(level_)
		<< " "
		<< entry.fileName << ":" << entry.lineNumber
		<< "]  ";
	
	return *this;
}


const char* Logger::logLevelToString(LogLevel level)
{
	switch (level) {
	case LOG_LVL_NONE:    return "NONE";
	case LOG_LVL_ERROR:   return "ERROR";
	case LOG_LVL_WARNING: return "WARNING";
	case LOG_LVL_DEBUG:   return "DEBUG";
	default:              return "UNDEFINED";
	}
}


Logger& Logger::getLogger(LogLevel level)
{
	if (level > LOG_LVL_DEBUG)
		return levels_[LOG_LVL_NONE];
	
	return levels_[level];
}


void Logger::addOutput(LogLevel level, Ref<Output> output)
{
	if (level >= (int)levels_.size())
		level = (LogLevel)(levels_.size() - 1);
	
	for (int l = LOG_LVL_ERROR; l <= level; l++) {
		levels_[l].addOutput(output);
	}
}


void Logger::addOutput(LogLevel level, string fileName)
{
	Ref<Output> output;
	
	if (fileName == "-") {
		output = new StandardErrorOutput();
	} else {
		output = new FileOutput(fileName, ios_base::out | ios_base::app);
	}
	
	addOutput(level, output);
}


/**
 *
 */
void Logger::clearConfig()
{
	FOR_EACH(levels_, lvl) {
		lvl->clearOutputs();
	}
}


/**
 *
 */
void Logger::defaultConfig(string fileName)
{
	clearConfig();
	
#ifdef NDEBUG
	addOutput(LOG_LVL_ERROR, "-");
	addOutput(LOG_LVL_ERROR, fileName);
#else
	addOutput(LOG_LVL_DEBUG, "-");
	addOutput(LOG_LVL_DEBUG, fileName);
#endif
}


/**
 *
 */
void Logger::defaultConfig()
{
	clearConfig();
	
#ifdef NDEBUG
	addOutput(LOG_LVL_ERROR, "-");
#else
	addOutput(LOG_LVL_DEBUG, "-");
#endif
}


vector<string> Logger::getBacktrace()
{
	void* buffer[30];
	size_t size;
	char** strings;

	size = backtrace(buffer, 30);
	strings = backtrace_symbols(buffer, size);
	
	vector<string> result;
	for (int i = 0; i < (int)size; i++) {
		stringstream s;
		s << strings[i];
		result.push_back(s.str());
	}
	
	free(strings);
	
	return result;
}


ostream& operator << (ostream& output, const LogTime& tm)
{
	time_t t = time(NULL);
	char buffer[80];
	int length = strftime(buffer, 80, "%A %H:%M:%S", localtime(&t));
	if (length > 0)
		output << buffer;
	else
		output << "#error#";
	return output;
}


} // namespace cppapp

