
#include <iostream>
using namespace std;

#include <cppapp/cppapp.h>
using namespace cppapp;

#include "ObjectTest.h"
#include "JSONTest.h"
#include "StringUtilsTest.h"
#include "DynObjectTest.h"
#include "InjectorTest.h"


class BacktraceTest : public TestCase {
public:
	BacktraceTest()
	{
		TEST_ADD(BacktraceTest, testPrint);
	}
	
	void testPrint()
	{
		ostringstream out;
		
		Backtrace bt;
		bt.get();
		bt.print(out);
	}
};

RUN_SUITE(BacktraceTest);


RUN_SUITE(TestTestCase);

TESTAPP_BOOTSTRAP;


