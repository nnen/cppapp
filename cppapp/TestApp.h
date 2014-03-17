/**
 * \file   TestApp.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-06-13
 *
 * \brief  Header file for the TestApp class.
 */

#ifndef TESTAPP_BX7LDD7T
#define TESTAPP_BX7LDD7T


#include "Test.h"
#include "AppBase.h"


namespace cppapp {


/**
 * \todo Write documentation for class TestApp.
 */
class TestApp : public AppBase {
protected:
	virtual void setUp();
	virtual int onRun();
};


#define TESTAPP_BOOTSTRAP \
	int main(int argc, char *argv[]) \
	{ \
		TestApp app; \
		return app.run(argc, argv); \
	}


} /* namespace cppapp */


#endif /* end of include guard: TESTAPP_BX7LDD7T */

