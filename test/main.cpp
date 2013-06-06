
#include <iostream>
using namespace std;

#include <cppapp/cppapp.h>
using namespace cppapp;


RUN_SUITE(TestTestCase);


class App : public AppBase {
protected:
	virtual int onRun()
	{
		TestSuite::runDefault();
		
		return 0;
	}
};


CPPAPP_BOOTSTRAP

