/**
 * \file   ObjectTest.h
 * \author Jan Milík <milikjan@fit.cvut.cz>
 * \date   2013-06-26
 *
 * \brief  Header file for the ObjectTest class.
 */

#ifndef OBJECTTEST_LCLNOC97
#define OBJECTTEST_LCLNOC97


#include <cppapp/cppapp.h>
using namespace cppapp;


/**
 * \todo Write documentation for class ObjectTest.
 */
class ObjectTest : public TestCase {
public:
	/**
	 * Constructor.
	 */
	ObjectTest()
	{
		TEST_ADD(ObjectTest, testStackAllocation);
		TEST_ADD(ObjectTest, testHeapAllocation);
	}
	
	void testStackAllocation()
	{
		ObjectTest testCase;
		testCase.checkHealth();
		testCase.claim();
		testCase.checkHealth();
	}
	
	void testHeapAllocation()
	{
		Ref<Object> obj = new Object();
		obj->checkHealth();
	}
};

RUN_SUITE(ObjectTest);


#endif /* end of include guard: OBJECTTEST_LCLNOC97 */

