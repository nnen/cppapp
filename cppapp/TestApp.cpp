/**
 * \file   TestApp.cpp
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-06-15
 * 
 * \brief  Implementation file for the TestApp class.
 */

#include "TestApp.h"


namespace cppapp {


void TestApp::setUp()
{
	AppBase::setUp();
	
	options().add('d',
			    "debug_tests",
			    "Debug unit tests.");
}


int TestApp::onRun()
{
	AppBase::onRun();
	
	if ((bool)options().get('d')) {
		TestSuite::getDefaultSuite().debug();
	} else {
		TestSuite::getDefaultSuite().run();
	}
	
	return EXIT_SUCCESS;
}


} // namespace cppapp

