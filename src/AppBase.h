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

#include "Object.h"
#include "Options.h"


namespace cppapp {


class AppBase : public Object {
private:
	Options options_;
	
	AppBase(const AppBase& other);

protected:
	Options&            getOptions() { return options_; }
	Options::Arguments& getArguments() { return options_.args(); }
	
	virtual void printUsage(std::ostream& out);
	virtual void setUp() {}
	virtual int run() { return EXIT_SUCCESS; }

public:
	AppBase();
	virtual ~AppBase();
	
	int run(int argc, char* argv[]);
};


template<class T>
int main(int argc, char* argv[])
{
	T app;
	return app.run(argc, argv);
}


} // namespace cppapp


#endif /* end of include guard: APP_NA5YPRSL */

