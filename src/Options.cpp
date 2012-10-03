/**
 * \file   Options.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-03-23
 * 
 * \brief Implementation file for the Options class.
 */

#include "Options.h"


namespace cppapp {


#define ERR(expr) { stringstream s; s << expr; error(s.str()); }


void Options::error(string message)
{
	cerr << message << endl;
	valid = false;
}


Options::Options()
{
	valid = true;
}


Options::~Options()
{
	//FOR_EACH(options, opt) {
	//	delete opt->second;
	//	opt->second = NULL;
	//}
}


void Options::add(char option, string defaultValue, const char* metavar)
{
	options[option] = Option(option, defaultValue, metavar);
}


void Options::add(char option, const char * defaultValue, const char* metavar)
{
	add(option, string(defaultValue), metavar);
}


void Options::add(char option, bool takesArgument, const char* metavar)
{
	options[option] = Option(option, takesArgument, metavar);
}


void Options::add(char option)
{
	add(option, false, "");
}


void Options::parse(int argc, char *argv[])
{
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


void Options::printUsage(ostream& output)
{
	output << executable;
	FOR_EACH(options, pair) {
		output << " ";
		pair->second.printUsage(output);
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

