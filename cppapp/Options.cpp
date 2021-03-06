/**
 * \file   Options.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-03-23
 * 
 * \brief Implementation file for the Options class.
 */


#include <cstring>

#include "Options.h"


namespace cppapp {


#define ERR(expr) { stringstream s; s << expr; error(s.str()); }


////////////////////////////////////////////////////////////////////////////////
// Option
////////////////////////////////////////////////////////////////////////////////


void Option::setConfigKey(Ref<DynObject> config) const
{
	if (configKey.size() == 0)
		return;
	
	Ref<DynObject> value;
	if (takesArgument) {
		if (!isSet) return;
		value = new DynString(TextLoc("<command line>"), argument);
	} else {
		value = new DynBoolean(TextLoc("<command line>"), isSet);
	}
	
	vector<string> parts;
	Strings::split(configKey, ".", &parts);
	
	Ref<DynObject> target = config;
	for (int i = 0; i < (parts.size() - 1); i++) {
		target = target->getStrItem(parts[i]);
		if (target->isError()) {
			return;
		}
	}
	
	target->setStrItem(parts[parts.size() - 1], value);
}


////////////////////////////////////////////////////////////////////////////////
// Options
////////////////////////////////////////////////////////////////////////////////


void Options::error(string message)
{
	cerr << message << endl;
	valid = false;
}


Options::Options()
	: usage(""), valid(true)
{
}


Options::~Options()
{
	//FOR_EACH(options, opt) {
	//	delete opt->second;
	//	opt->second = NULL;
	//}
}


void Options::add(char        option,
			   string      defaultValue,
			   const char* metavar,
			   const char* configKey,
			   const char* description)
{
	options[option] = Option(option,
						defaultValue,
						metavar,
						configKey,
						description);
	
	configKeys[configKey] = option;
}


void Options::add(char        option,
			   const char* defaultValue,
			   const char* metavar,
			   const char* configKey,
			   const char* description)
{
	add(option, string(defaultValue), metavar, configKey, description);
}


void Options::add(char        option,
			   const char* configKey,
			   const char* description)
{
	options[option] = Option(option,       // letter
						false,        // takes argument
						"",           // metavar
						configKey,    // config key
						description); // description
}


void Options::parse(int argc, char *argv[])
{
	this->argc = argc;
	this->argv = argv;
	
	stringstream s;
	s << ":";
	FOR_EACH(options, opt) {
		s << opt->second.letter;
		if (opt->second.takesArgument)
			s << ':';
	}
	
	executable = string(argv[0]);
	
	int c;
	while ((c = getopt(argc, argv, s.str().c_str())) != -1) {
		Map::iterator opt;
		if ((c == '?') || (c == ':'))
			opt = options.find(optopt);
		else
			opt = options.find(c);
		if (opt == options.end()) {
			ERR("Unknown option: " << (char)optopt);
			continue;
		}
		if ((c == '?') || (c == ':')) {
			ERR("Option \"" << opt->second.letter << "\" requires argument.");
		} else {
			opt->second.set(optarg);
		}
	}
	
	for (int index = optind; index < argc; index++) {
		arguments.push_back(argv[index]);
	}
}


Option& Options::get(int opt)
{
	return options[opt];
}


Option& Options::get(string configKey)
{
	return get(configKeys[configKey]);
}


void Options::setConfigKeys(Ref<Config> config) const
{
	FOR_EACH(options, it) {
		it->second.setConfigKey(config);
	}
}


void Options::setConfigKeys(Ref<DynObject> config) const
{
	FOR_EACH(options, it) {
		it->second.setConfigKey(config);
	}
}


void Options::printUsage(ostream& output)
{
	output << "Usage: " << executable;
	FOR_EACH(options, pair) {
		output << " ";
		pair->second.printUsage(output);
	}
	output << " " << usage;
	output << endl << "Options:" << endl;
	FOR_EACH(options, pair) {
		if (strcmp(pair->second.description, "") == 0)
			continue;
		output << "   -" << pair->second.letter;
		output << "  " << pair->second.description << endl;
	}
}


void Options::dump(ostream& output)
{
	output << "Options valid: " << (valid ? "true" : "false") << endl;
	
	FOR_EACH(options, opt) {
		output << "-" << opt->second.letter << ": ";
		if (opt->second.isSet) {
			if (opt->second.takesArgument)
				output << opt->second.argument;
			else
				output << "true ";
		} else {
			if (opt->second.takesArgument) {
				output << opt->second.argument;
			} else {
				output << "false ";
			}
		}
		output << endl;
	}
	
	output << endl << "Positional arguments:" << endl;
	FOR_EACH(arguments, a) {
		output << "> " << *a << endl;
	}
}


} // namespace cppapp

