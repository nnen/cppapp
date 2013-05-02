/**
 * \file   AppBase.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-10-03
 * 
 * \brief Implementation file for the AppBase class.
 */

#include "AppBase.h"

#include <sstream>


namespace cppapp {


/**
 *
 */
string AppBase::getDefaultConfigFile() const
{
	ostringstream s;
	s << getenv("HOME") <<
		"/." <<
		pathBasename(options_.getExecutable());
	return s.str();
}


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
	options().add('o',
			    "-",
			    "OUTPUT_FILE",
			    CPPAPP_OUTPUT_FILE_CFG_KEY,
			    "Write output to file OUTPUT_FILE.");
	options().add('h', "", "Print this help.");
}


/**
 *
 */
int AppBase::onRun()
{
	if ((bool)options().get('h')) {
		printUsage(std::cout);
		exit(EXIT_SUCCESS);
	}
	
	VAR(out, config_->get(CPPAPP_OUTPUT_FILE_CFG_KEY));
	if (out.isNotNull()) {
		output_ = new FileOutput(out->asString());
	}
	
	//Option out = options().get('o');
	//if ((bool)out) {
	//	output_ = new FileOutput((string)out);
	//}
	
	return EXIT_SUCCESS;
}


/**
 * Constructor.
 */
AppBase::AppBase() :
	config_(new Config()),
	output_(new StandardOutput())
{
}


int AppBase::run(int argc, char* argv[])
{
	setUp();
	
	Logger::defaultConfig();
	
	options_.parse(argc, argv);
	if (!options_.isValid()) {
		printUsage(std::cerr);
		return EXIT_FAILURE;
	}
	
	string configFileName = config_->get("config_file",
								  getDefaultConfigFile())->asString();
	LOG_EXPR(configFileName);
	ConfigParser parser(config_);
	parser.parse(new FileInput(configFileName));

	options_.setConfigKeys(config_);
	
	return onRun();
}


} // namespace cppapp

