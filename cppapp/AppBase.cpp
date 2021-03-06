/**
 * \file   AppBase.cpp
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-10-03
 * 
 * \brief Implementation file for the AppBase class.
 */

#include "AppBase.h"
#include "Logger.h"

#include <sstream>


namespace cppapp {


/**
 *
 */
string AppBase::getDefaultConfigFile()
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


void AppBase::readConfig()
{
	if (!readConfig_)
		return;
	
	string configFileName = config_->get(CPPAPP_CONFIG_FILE_CFG_KEY,
								  getDefaultConfigFile())->asString();
	
	Ref<FileInput> configInput = new FileInput(configFileName);
	if (configInput->exists()) {
		ConfigParser parser(config_);
		parser.parse(configInput);
		configInput->close();
	} else {
		LOG_WARNING("Configuration file " << configFileName << " does not exist.");
	}
	
	// Override the configuration read from the file by the command line
	// options.
	options_.setConfigKeys(config_);
}


/**
 *
 */
void AppBase::setUp()
{
	options().add('c',
			    "-",
			    "CONFIG_FILE",
			    CPPAPP_CONFIG_FILE_CFG_KEY,
			    "Use the specified config file instead of the default.");
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
	readConfig_(true),
	config_(Config::globalConfig()),
	output_(new StandardOutput())
{
}


/**
 * Constructor.
 */
AppBase::AppBase(bool readConfig) :
	readConfig_(readConfig),
	config_(Config::globalConfig()),
	output_(new StandardOutput())
{
}


int AppBase::run(int argc, char* argv[])
{
	// Call setUp hook.
	setUp();
	
	Logger::defaultConfig();
	
	// Parse command line options
	options_.parse(argc, argv);
	if (!options_.isValid()) {
		printUsage(std::cerr);
		return EXIT_FAILURE;
	}
	if (options_.get(CPPAPP_CONFIG_FILE_CFG_KEY).isSet)
		options_.get(CPPAPP_CONFIG_FILE_CFG_KEY).setConfigKey(config_);
	
	// Parse configuration file
	readConfig();
	
	// Run the actual application.
	return onRun();
}


} // namespace cppapp

