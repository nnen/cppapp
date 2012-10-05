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
#include "Options.h"
#include "Output.h"


namespace cppapp {


class AppBase : public Object {
private:
	Options     options_;
	Ref<Output> output_;
	
	AppBase(const AppBase& other);

protected:
	Options&            options() { return options_; }
	Options::Arguments& args() { return options_.args(); }
	
	Ref<Output>         output() { return output_; }
	
	virtual void printUsage(std::ostream& out);
	virtual void setUp();
	virtual int run();

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

