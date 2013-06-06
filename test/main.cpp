
#include <iostream>
using namespace std;

#include <cppapp/cppapp.h>
using namespace cppapp;


class App : public AppBase {
protected:
	virtual int onRun()
	{
		RUN_SUITE(TestTestCase);
		TestSuite::runDefault();
		
		return 0;
	}
};


CPPAPP_BOOTSTRAP

