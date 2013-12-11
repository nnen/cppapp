
#include <iostream>
using namespace std;

#include <cppapp/cppapp.h>
using namespace cppapp;

#include "ObjectTest.h"
#include "JSONTest.h"


class BacktraceTest : public TestCase {
public:
	BacktraceTest()
	{
		TEST_ADD(BacktraceTest, testPrint);
	}
	
	void testPrint()
	{
		Backtrace bt;
		bt.get();
		bt.print(std::cout);
	}
};

RUN_SUITE(BacktraceTest);


RUN_SUITE(TestTestCase);

TESTAPP_BOOTSTRAP;


