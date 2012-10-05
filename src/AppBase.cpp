/**
 * \file   AppBase.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-10-03
 * 
 * \brief Implementation file for the AppBase class.
 */

#include "AppBase.h"


namespace cppapp {


void AppBase::printUsage(std::ostream& out)
{
	options_.printUsage(out);
	out << std::endl;
}


/**
 *
 */
void AppBase::setUp()
{
	options().add('o', true, "OUTPUT", "Write output to file OUTPUT.");
	options().add('h', "Print this help.");
}


/**
 *
 */
int AppBase::run()
{
	if ((bool)options().get('h')) {
		printUsage(std::cout);
		exit(EXIT_SUCCESS);
	}
	
	Option out = options().get('o');
	if ((bool)out) {
		output_ = new FileOutput((string)out);
	}
	
	return EXIT_SUCCESS;
}


/**
 * Constructor.
 */
AppBase::AppBase()
	: output_(new StandardOutput())
{
}


int AppBase::run(int argc, char* argv[])
{
	setUp();
	
	options_.parse(argc, argv);
	if (!options_.isValid()) {
		printUsage(std::cerr);
		return EXIT_FAILURE;
	}
	
	return run();
}


} // namespace cppapp

