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


namespace cppapp {


/**
 * \brief Base class for unix commnad-line applications.
 */
class AppBase : public Object {
private:
	Ref<Config> config_;
	
	Options     options_;
	Ref<Output> output_;
	
	AppBase(const AppBase& other);

protected:
	virtual string      getDefaultConfigFile() const;
	Ref<Config>         config() { return config_; }
	void                setConfig(Ref<Config> cfg) { config_ = cfg; }
	
	Options&            options() { return options_; }
	Options::Arguments& args() { return options_.args(); }
	
	Ref<Output>         output() { return output_; }
	void                setOutput(Ref<Output> output) { output_ = output; }
	
	virtual void        printUsage(std::ostream& out);
	virtual void        setUp();
	virtual int         onRun();

public:
	AppBase();
	virtual ~AppBase() {}
	
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


} // namespace cppapp


#endif /* end of include guard: APP_NA5YPRSL */

