/**
 * \file   AppBase.h
 * \author Jan Milik <milikjan@fit.cvut.cz>
 * \date   2012-10-03
 *
 * \brief Header file for the AppBase class.
 */

#ifndef APP_NA5YPRSL
#define APP_NA5YPRSL


#include <ostream>
#include <cstdlib>

#include "Object.h"
#include "Config.h"
#include "Options.h"
#include "Output.h"
#include "Logger.h"


#define CPPAPP_CONFIG_FILE_CFG_KEY "config_file"
#define CPPAPP_OUTPUT_FILE_CFG_KEY "output_file"


namespace cppapp {


/** \addtogroup app
 *
 * \brief Application framework
 *
 * @{
 */


/**
 * \brief Base class for unix commnad-line applications.
 */
class AppBase : public Object {
private:
	bool        readConfig_;
	Ref<Config> config_;
	
	Options     options_;
	Ref<Output> output_;
	
	AppBase(const AppBase& other);

protected:
	/**
	 * \brief Returns the name of the default config file that should be read by
	 *        the applications.
	 */
	virtual string      getDefaultConfigFile();
	/**
	 * \brief Returns the configuration object. 
	 */
	Ref<Config>         config() { return config_; }
	/**
	 * \brief Sets the configuration object.
	 *
	 * \param cfg the configuration object
	 */
	void                setConfig(Ref<Config> cfg) { config_ = cfg; }
	
	/**
	 * \brief Returns command-line options.
	 */
	Options&            options() { return options_; }
	/**
	 * \brief Returns positional command-line arguments.
	 */
	Options::Arguments& args() { return options_.args(); }
	
	/**
	 * \brief Get the output object (stdout by default).
	 */
	Ref<Output>         output() { return output_; }
	/**
	 * \brief Set the output object.
	 */
	void                setOutput(Ref<Output> output) { output_ = output; }
	
	/**
	 * \brief Prints the command-line usage to the given stream.
	 *
	 * This method is aware of the command-line options set through the
	 * \ref Options object (see \ref options()).
	 *
	 * \param out output stream to print the usage to
	 */
	virtual void        printUsage(std::ostream& out);
	/**
	 * \brief Reads the configuration file.
	 *
	 * This method is made virtual protected so that it can be overriden
	 * in a subclass.
	 */
	virtual void        readConfig();
	/**
	 * \brief Sets up command line options and does whatever initialization
	 *        is necessary before executing the application.
	 *
	 * This method is meant to be overriden in a subclass
	 * to do custom initialization.
	 */
	virtual void        setUp();
	/**
	 * \brief Performs the main application work.
	 *
	 * This method is meant to be overriden in a subclass.
	 *
	 * \returns the exit code for the process
	 */
	virtual int         onRun();

public:
	AppBase();
	/**
	 * \brief Constructor.
	 *
	 * \param readConfig if \c true, the class will attempt to read a config file
	 *                   specified by either command-line option or the default one
	 *                   given by \ref getDefaultConfigFile()
	 */
	AppBase(bool readConfig);
	virtual ~AppBase() {}
	
	/**
	 * \brief Executes the application in the current thread.
	 *
	 * \param argc number of command-line arguments
	 * \param argv array of C-strings containing the command-line arguments
	 * \returns exit code
	 */
	int run(int argc, char* argv[]);
};


template<class T>
int main(int argc, char* argv[])
{
	T app;
	return app.run(argc, argv);
}


#define CPPAPP_BOOTSTRAP \
int main(int argc, char* argv[]) \
{ \
	App app; \
	return app.run(argc, argv); \
}


/** @} */


} // namespace cppapp


#endif /* end of include guard: APP_NA5YPRSL */

