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

