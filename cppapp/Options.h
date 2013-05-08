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
#include "Config.h"


using namespace std;


namespace cppapp {


/**
 * \brief Represents a command-line option.
 */
struct Option {
	char        letter;
	bool        takesArgument;
	const char *argumentMetavar;
	const char *description;
	
	bool        isSet;
	string      argument;
	
	string      configKey;
	
	Option() :
		letter('?'),
		takesArgument(false),
		argumentMetavar(""),
		description(""),
		isSet(false),
		argument(),
		configKey("")
	{
	}
	
	Option(char letter,
		  bool takesArgument,
		  const char* metavar,
		  const char* configKey,
		  const char* description = "") :
		letter(letter),
		takesArgument(takesArgument),
		argumentMetavar(metavar),
		description(description),
		isSet(false),
		argument(),
		configKey(configKey)
	{	
	}
	
	Option(char letter,
		  string defaultValue,
		  const char* metavar,
		  const char* configKey,
		  const char* description = "") :
		letter(letter),
		takesArgument(true),
		argumentMetavar(metavar),
		description(description),
		isSet(false),
		argument(defaultValue),
		configKey(configKey)
	{
	}
	
	operator bool() const { return isSet; }
	operator string() const { return argument; }
	
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
	
	void setConfigKey(Ref<Config> config) const
	{
		if (configKey.size() == 0) return;
		
		if (takesArgument) {
			if (isSet) {
				config->set(new ConfigValue(configKey, argument));
			}
		} else {
			config->set(ConfigValue::make(configKey, isSet));
		}
	}
};


/**
 * \brief Defines and parses command line options.
 */
class Options {
public:
	typedef map<int, Option> Map;
	typedef vector<string> Arguments;

private:
	Options(const Options& other);
	
	int          argc;
	char**       argv;
	
	const char* usage;
	Map         options;
	Arguments   arguments;
	string      executable;
	bool        valid;
	
	void error(string message);
	
public:
	Options();
	~Options();
	
	void add(char        letter,
		    string      defaultValue,
		    const char* metavar,
		    const char* configKey,
		    const char* description = "");
	void add(char        letter,
		    const char* defaultValue,
		    const char* metavar,
		    const char* configKey,
		    const char* description = "");
	void add(char        letter,
		    const char* configKey,
		    const char* description = "");
	
	void setUsage(const char* usage) { this->usage = usage; }
	
	void parse(int argc, char *argv[]);
	
	Option& get(int opt);
	
	Arguments& args() { return arguments; } 
	string     getExecutable() const { return executable; }
	
	int getArgc() const { return argc; }
	char** getArgv() const { return argv; }
	
	bool isValid() const { return valid; }
	
	void setConfigKeys(Ref<Config> config) const;
	
	void printUsage(ostream& output);
	void dump(ostream& output);
};


} // namespace cppapp


#endif /* end of include guard: OPTIONS_G8OP4BZM */

