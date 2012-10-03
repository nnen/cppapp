/**
 * \file   Options.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-03-23
 *
 * \brief Header file for the Options class.
 */

#ifndef OPTIONS_G8OP4BZM
#define OPTIONS_G8OP4BZM


#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>

#include "utils.h"


using namespace std;


namespace cppapp {


/**
 * Represents a command-line option.
 */
struct Option {
	char        letter;
	bool        takesArgument;
	const char* argumentMetavar;
	
	bool        isSet;
	string      argument;
	
	Option() :
		letter('?'), takesArgument(false), argumentMetavar(""), isSet(false), argument()
	{
	}
	
	Option(char letter, bool takesArgument, const char* metavar) :
		letter(letter), takesArgument(takesArgument), argumentMetavar(metavar), isSet(false), argument()
	{	
	}
	
	Option(char letter, string defaultValue, const char* metavar) :
		letter(letter), takesArgument(true), argumentMetavar(metavar), isSet(false), argument(defaultValue)
	{
	}
	
	operator bool() const { return isSet; }
	
	void set(char * arg)
	{
		isSet = true;
		if (takesArgument)
			argument = arg;
	}
	
	long int toLong() const
	{
		return strtol(argument.c_str(), NULL, 10);
	}
	
	void printUsage(ostream& output)
	{
		output << "-" << letter;
		if (takesArgument) {
			output << " <" << argumentMetavar << ">";
		}
	}
};


/**
 * Represents a set of command line options.
 */
class Options {
public:
	typedef map<int, Option> Map;
	typedef vector<string> Arguments;

private:
	Options(const Options& other);
	
	Map       options;
	Arguments arguments;
	string    executable;
	bool      valid;
	
	void error(string message);
	
public:
	Options();
	~Options();
	
	void add(char letter, string defaultValue, const char* metavar);
	void add(char letter, const char * defaultValue, const char* metavar);
	void add(char letter, bool takesArgument, const char* metavar);
	void add(char letter);
	
	void parse(int argc, char *argv[]);
	
	Option& get(int opt);
	
	Arguments& args() { return arguments; } 
	
	bool isValid() const { return valid; }
	
	void printUsage(ostream& output);
	void dump(ostream& output);
};


} // namespace cppapp


#endif /* end of include guard: OPTIONS_G8OP4BZM */

